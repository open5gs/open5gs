/*
 * CHF subscriber quota profile — Redux-connected document container.
 *
 * Responsibilities:
 *   1. Fetch an existing ChfSubscriber document from the REST API afresh
 *      each time an editor opens on it, and open the form once it is here.
 *   2. Convert raw byte values stored in MongoDB → human-readable display form
 *      for Edit.js (dbToForm: totalQuotaBytes → totalVolume + volumeUnit).
 *   3. Convert the form's display values → raw bytes + canonical seconds before
 *      persisting (formToDb), so the Mongoose model and C db-handler.c always
 *      see consistent totalQuotaBytes / validitySeconds fields.
 *   4. Dispatch create / update actions and show success / error notifications.
 *   5. Save a shared pool - a new one ('create-pool'), or an existing one,
 *      which has members - through the TMF API instead (modules/crud/
 *      chf-pool.js): that is where the pool rules and the CHF notifications
 *      are.
 *
 * Unit conversion (binary prefixes, IEC 80000-13):
 *   1 MB = 1 048 576 bytes   (2^20)
 *   1 GB = 1 073 741 824 bytes (2^30)
 *   1 TB = 1 099 511 627 776 bytes (2^40)
 *
 * 3GPP: TS 23.255 §5.2   — CHF subscriber management
 *       TS 32.291 §6.3.1 — ChargingDataRequest: grantedUnit drives the quota
 *       TS 32.291 §6.3.2.2 — TIME_LIMIT / VOLUME_LIMIT triggers
 */

import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { isEqual } from 'lodash/lang';

import NProgress from 'nprogress';

import {
  MODEL,
  fetchChfSubscriber,
  createChfSubscriber,
  updateChfSubscriber,
} from 'modules/crud/chf-subscriber';
import { clearActionStatus } from 'modules/crud/actions';
import { select, selectActionStatus } from 'modules/crud/selectors';
import * as Notification from 'modules/notification/actions';

import { ChfSubscriber } from 'components';
import {
  DEFAULT_QUOTA_FORM as defaultFormData,
  DEFAULT_POOL_FORM as defaultPoolForm,
  dbToForm,
  formToDb,
  poolFormToTmf,
} from 'helpers/chf-quota-convert';
import { createPool, updatePool, poolError, isPoolId } from 'modules/crud/chf-pool';

/* ------------------------------------------------------------------ */
/* Document container                                                   */
/* ------------------------------------------------------------------ */
class Document extends Component {
  static propTypes = {
    action: PropTypes.string,
    visible: PropTypes.bool,
    onHide: PropTypes.func,
  };

  state = {
    formData: defaultFormData,
    /* An editor opened on an existing document stays hidden until that
     * document has been read afresh: see componentWillReceiveProps. */
    waiting: false,
  };

  componentWillMount() {
    const { chfSubscriber, dispatch } = this.props;
    if (chfSubscriber.needsFetch) {
      dispatch(chfSubscriber.fetch);
    }
  }

  componentWillReceiveProps(nextProps) {
    const { chfSubscriber, status } = nextProps;
    const { dispatch, action, onHide } = this.props;
    const opening = nextProps.visible && !this.props.visible;

    /*
     * An editor reads its document afresh each time it opens, and the form
     * is shown only once that read is back.  The copy already in the store
     * came with the list and can be ten minutes old; a pool's members may
     * have changed since, through the TMF API or a SIM swap, and saving a
     * pool sends its whole member list, so a stale copy would undo that
     * change.  The form also takes its data only as it opens (Shared/Form),
     * so it must not open before the data is here: a pool opened from the
     * CHF Quota view opens as the CHF Shared Pools view mounts.
     *
     * fetchedAt is stamped when a request starts, so one at or after the
     * opening is this read.  Once it is in, the form stays open: a list
     * request still in flight may land later with an older stamp.
     */
    if (opening && nextProps.action === 'update' && chfSubscriber.fetch) {
      this.openedAt = Date.now();
      this.setState({ waiting: true });
      dispatch(chfSubscriber.fetch);
    } else if (chfSubscriber.needsFetch) {
      dispatch(chfSubscriber.fetch);
    }

    if (!nextProps.visible) {
      if (this.state.waiting) this.setState({ waiting: false });
    } else if (!opening && this.state.waiting &&
               nextProps.fetchedAt >= this.openedAt) {
      this.setState({ waiting: false });
    }

    /*
     * Convert raw bytes from DB → display unit for the form.
     * dbToForm() handles both the new totalQuotaBytes field and legacy
     * volumeUlBytes + volumeDlBytes (backward compat with older records).
     *
     * Only load the form when the modal opens or the document behind it
     * actually changes.  Both CHF pages poll on a timer, so every few
     * seconds an unrelated store update re-renders this connected component;
     * resetting formData unconditionally would discard whatever the operator
     * had typed mid-edit.
     */
    const nextForm = chfSubscriber.data
      ? dbToForm(chfSubscriber.data)
      : (nextProps.action === 'create-pool' ? defaultPoolForm : defaultFormData);

    if ((nextProps.visible && !this.props.visible) ||
        !isEqual(nextForm, this.loadedForm)) {
      this.loadedForm = nextForm;
      this.setState({ formData: nextForm });
    }

    if (status.response) {
      NProgress.configure({ parent: 'body', trickleSpeed: 5 });
      NProgress.done(true);

      const message =
        action === 'create'
          ? 'New CHF quota profile created'
          : `${status.id} quota profile updated`;

      dispatch(Notification.success({ title: 'CHF Quota', message }));
      dispatch(clearActionStatus(MODEL, action));
      onHide();
    }

    if (status.error) {
      NProgress.configure({ parent: 'body', trickleSpeed: 5 });
      NProgress.done(true);

      const response = ((status || {}).error || {}).response || {};
      let title = 'Unknown Code';
      let message = 'Unknown Error';
      if (response.data && response.data.name && response.data.message) {
        title = response.data.name;
        message = response.data.message;
      } else {
        title = response.status;
        message = response.statusText;
      }

      dispatch(
        Notification.error({
          title,
          message,
          autoDismiss: 0,
          action: { label: 'Dismiss', callback: () => onHide() },
        })
      );
      dispatch(clearActionStatus(MODEL, action));
    }
  }

  /*
   * A pool, new or existing, is saved through the TMF API.  Its answer is
   * the outcome: on success the list is refreshed by onHide(); on failure
   * the API's own message is shown - a SIM already in another pool, too
   * many SIMs for charging.pool.max_members - and the form stays open.
   */
  savePool = async (formData, id) => {
    const { dispatch, action, onHide } = this.props;
    const create = action === 'create-pool';
    try {
      const res = create
        ? await createPool(poolFormToTmf(formData, true))
        : await updatePool(id, poolFormToTmf(formData, false));
      const n = ((res.data || {}).logicalResource || []).length;
      dispatch(Notification.success({
        title: 'CHF Quota',
        message: create
          ? `Shared pool ${res.data.id} created for ${n} SIMs`
          : `Shared pool ${id} updated: ${n} SIMs`,
      }));
      onHide();
    } catch (error) {
      const { title, message } = poolError(error);
      dispatch(Notification.error({
        title, message, autoDismiss: 0, action: { label: 'Dismiss' },
      }));
    } finally {
      NProgress.configure({ parent: 'body', trickleSpeed: 5 });
      NProgress.done(true);
    }
  };

  handleSubmit = formData => {
    const { dispatch, action } = this.props;

    NProgress.configure({ parent: '#nprogress-base-form', trickleSpeed: 5 });
    NProgress.start();

    /*
     * Which document this is comes from what was opened, not from the form:
     * the form only gives back the fields it shows, and a pool opened from a
     * SIM's quota on the CHF Quota view has lost members and bucketId by the
     * time it is saved.
     */
    const { bucketId, chfSubscriber } = this.props;
    const opened = chfSubscriber.data || {};
    if (action === 'create-pool' ||
        (action === 'update' &&
         (isPoolId(bucketId) || Array.isArray(opened.members)))) {
      this.savePool(formData, bucketId || opened.bucketId);
      return;
    }

    /*
     * Convert display-unit fields (totalVolume, volumeUnit, validityPeriod)
     * to raw bytes (totalQuotaBytes) and canonical seconds (validitySeconds)
     * before persisting.  This keeps the Mongoose model schema consistent and
     * ensures db-handler.c reads the correct camelCase field names.
     */
    if (action === 'update' && (bucketId || opened.bucketId))
      formData = Object.assign({}, formData,
          { bucketId: bucketId || opened.bucketId });
    const dbData = formToDb(formData);

    if (action === 'create') {
      dispatch(createChfSubscriber({}, dbData));
    } else if (action === 'update') {
      dispatch(updateChfSubscriber(dbData.bucketId || dbData.supi, {}, dbData));
    } else {
      throw new Error(`Action type '${action}' is invalid.`);
    }
  };

  handleError = errors => {
    const { dispatch } = this.props;
    errors.map(error =>
      dispatch(
        Notification.error({ title: 'Validation Error', message: error.stack })
      )
    );
  };

  render() {
    const { handleSubmit, handleError } = this;
    const { visible, action, status, chfSubscriber, onHide } = this.props;

    return (
      <ChfSubscriber.Edit
        visible={visible && !this.state.waiting}
        action={action}
        formData={this.state.formData}
        isLoading={chfSubscriber.isLoading && !status.pending}
        onHide={onHide}
        onSubmit={handleSubmit}
        onError={handleError}
      />
    );
  }
}

Document = connect((state, props) => ({
  chfSubscriber: select(fetchChfSubscriber(props.bucketId || props.supi),
      state.crud),
  fetchedAt: state.crud.getIn(
      [MODEL, 'byId', props.bucketId || props.supi, 'fetchedAt']),
  status: selectActionStatus(MODEL, state.crud, props.action),
}))(Document);

export default Document;

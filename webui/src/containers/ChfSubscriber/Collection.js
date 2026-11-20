/*
 * CHF subscriber quota-profile collection.
 *
 * Scale hardening for a large provisioned-subscriber base:
 *   - Server-side pagination (?limit/?skip, sorted by SUPI): only one page is
 *     ever fetched and rendered, never the whole chf.subscribers collection.
 *   - Server-side SUPI search (?query, prefix-anchored + regex-escaped) so an
 *     operator finds one subscriber among thousands without downloading them.
 *   - X-Total-Count drives an "N of TOTAL" indicator and the Next control.
 *
 * Structure: a stateful, connected parent owns page/search + the create /
 * edit / delete modals; a connected child selects exactly the page named by
 * queryParams (a page/search change flips crud "needsFetch" and refetches).
 *
 * Two views use it.  CHF Quota (the default) lists every bucket, and a SUPI
 * search finds that SIM's own buckets and its shared pool; a pool's row
 * there links to the CHF Shared Pools view (`pools`), which lists pools only and
 * is where they are created, changed and deleted - through the TMF API.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import {
  MODEL,
  fetchChfSubscribers,
  deleteChfSubscriber,
} from 'modules/crud/chf-subscriber';
import { clearActionStatus } from 'modules/crud/actions';
import { select, selectActionStatus } from 'modules/crud/selectors';
import {
  buildSubscriberParams, buildPoolParams, totalFromOtherInfo,
} from 'helpers/chf-query';
import { openPool } from 'modules/sidebar';
import { deletePool, isPoolId, poolError } from 'modules/crud/chf-pool';
import * as Notification from 'modules/notification/actions';
import GroupAddIcon from 'react-icons/lib/md/group-add';

import {
  Layout,
  ChfSubscriber,
  Spinner,
  FloatingButton,
  Blank,
  Dimmed,
  Confirm,
  ListToolbar,
} from 'components';
import ChfStats from 'containers/ChfStats';

import Document from './Document';

const PAGE_SIZE = 25;
const SEARCH_DEBOUNCE_MS = 400;

/* ------------------------------------------------------------------ */
/* Connected data view — selects and renders one page of subscribers.  */
/* ------------------------------------------------------------------ */
class SubscriberData extends Component {
  componentWillMount() {
    this.maybeFetch(this.props);
  }

  componentWillReceiveProps(nextProps) {
    this.maybeFetch(nextProps);
  }

  maybeFetch(props) {
    /* Only fetch a page when the view is active (a search or Browse) —
     * the default landing is stats-only and must not pull the collection. */
    if (props.active && props.chfSubscribers.needsFetch) {
      props.dispatch(props.chfSubscribers.fetch);
    }
  }

  render() {
    const {
      active, chfSubscribers, deletedSupi, onEdit, onDelete, onBrowse,
      page, pageSize, search, onSearchChange, onPrev, onNext,
      onManagePool, pools, onCreatePool,
    } = this.props;
    const { isLoading, data, otherInfo } = chfSubscribers;
    const total = totalFromOtherInfo(otherInfo);

    return (
      <div>
        <ListToolbar
          showSearch
          search={search}
          searchPlaceholder={pools
            ? "Search by pool id or a member's SUPI, or a prefix (pool-…, imsi-99970 …)"
            : 'Search by SUPI or pool id, or a prefix (imsi-99970 …, pool-…)'}
          onSearchChange={onSearchChange}
          page={page}
          pageSize={pageSize}
          count={active ? data.length : 0}
          total={active ? total : undefined}
          onPrev={onPrev}
          onNext={onNext}
        />

        {!active ? (
          <Blank
            visible
            title="BROWSE ALL SUBSCRIBERS"
            body="Search by SUPI (or a prefix) above, or browse the full list."
            onTitle={onBrowse}
          />
        ) : (
          <div>
            <ChfSubscriber.List
              chfSubscribers={data}
              deletedSupi={deletedSupi}
              onEdit={onEdit}
              onDelete={onDelete}
              onManagePool={onManagePool}
              pools={pools}
            />
            {isLoading && <Spinner md />}
            <Blank
              visible={!isLoading && !(data.length > 0)}
              title={pools ? 'NEW SHARED POOL' : 'NO MATCHING SUBSCRIBERS'}
              body={pools
                ? (search.trim()
                    ? 'No shared pool has this id, or this SIM as a member.'
                    : 'No shared pools here yet: several SIMs sharing one allowance.')
                : 'No CHF quota profiles match this view.'}
              onTitle={pools ? onCreatePool : undefined}
            />
          </div>
        )}
      </div>
    );
  }
}

SubscriberData.propTypes = {
  active: PropTypes.bool.isRequired,
  chfSubscribers: PropTypes.object.isRequired,
  dispatch: PropTypes.func.isRequired,
  deletedSupi: PropTypes.string,
  onEdit: PropTypes.func.isRequired,
  onDelete: PropTypes.func.isRequired,
  onBrowse: PropTypes.func.isRequired,
  page: PropTypes.number.isRequired,
  pageSize: PropTypes.number.isRequired,
  search: PropTypes.string.isRequired,
  onSearchChange: PropTypes.func.isRequired,
  onPrev: PropTypes.func.isRequired,
  onNext: PropTypes.func.isRequired,
  onManagePool: PropTypes.func,
  onCreatePool: PropTypes.func,
  pools: PropTypes.bool,
};

const ConnectedSubscriberData = connect((state, ownProps) => ({
  chfSubscribers: select(fetchChfSubscribers(ownProps.queryParams), state.crud),
}))(SubscriberData);

/* ------------------------------------------------------------------ */
/* Stateful parent — owns page/search + create/edit/delete modals.     */
/* ------------------------------------------------------------------ */
class Collection extends Component {
  state = {
    page: 0,
    searchInput: '',
    searchApplied: '',
    browse: false,   // "Browse all" was clicked (list without a search)
    document: {
      action: '',
      visible: false,
      dimmed: false,
    },
    confirm: {
      visible: false,
      /* The bucket to delete: a SUPI for a subscriber's catch-all, or a
       * scoped bucket's own id. */
      bucketId: '',
    },
  };

  componentWillUnmount() {
    if (this._searchTimer) clearTimeout(this._searchTimer);
  }

  /*
   * Sent here from a SIM's quota on the CHF Quota view: open that pool.  A
   * view that is not shown is not mounted, so the request is usually here
   * when this view mounts; it can also arrive while it is shown.
   */
  componentDidMount() {
    this.openRequestedPool(this.props.openPoolId);
  }

  openRequestedPool(id) {
    if (!this.props.pools || !id) return;
    this.documentHandler.actions.update(id);
    this.props.dispatch(openPool(null));
  }

  componentWillReceiveProps(nextProps) {
    const { status } = nextProps;
    const { dispatch, pools } = this.props;

    if (pools && nextProps.openPoolId &&
        nextProps.openPoolId !== this.props.openPoolId)
      this.openRequestedPool(nextProps.openPoolId);

    /* The REST deletes are the CHF Quota view's; both views see their
     * status, and only one may report it. */
    if (pools) return;

    if (status.response) {
      dispatch(
        Notification.success({
          title: 'CHF Quota',
          message: `${status.id} has been deleted`,
        })
      );
      dispatch(clearActionStatus(MODEL, 'delete'));
      /* Refresh the current page so the deleted row disappears. */
      dispatch(fetchChfSubscribers(this.params()));
    }

    if (status.error) {
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
          action: { label: 'Dismiss' },
        })
      );
      dispatch(clearActionStatus(MODEL, 'delete'));
    }
  }

  params() {
    return (this.props.pools ? buildPoolParams : buildSubscriberParams)(
      this.state.page, PAGE_SIZE, this.state.searchApplied);
  }

  isActive() {
    /* Pools are few: the CHF Shared Pools view lists them straight away. */
    return this.props.pools ||
        this.state.searchApplied !== '' || this.state.browse;
  }

  handleManagePool = bucketId => this.props.dispatch(openPool(bucketId));

  handleSearchChange = (value) => {
    this.setState({ searchInput: value });
    if (this._searchTimer) clearTimeout(this._searchTimer);
    this._searchTimer = setTimeout(() => {
      this.setState({ searchApplied: value.trim(), page: 0 });
    }, SEARCH_DEBOUNCE_MS);
  };

  handleBrowse = () => this.setState({ browse: true, page: 0 });

  handlePrev = () => this.setState(s => ({ page: Math.max(0, s.page - 1) }));
  handleNext = () => this.setState(s => ({ page: s.page + 1 }));

  documentHandler = {
    show: (action, payload) => {
      this.setState({
        document: { action, visible: true, dimmed: true, ...payload },
      });
    },
    hide: () => {
      /* Refresh the page after a create/edit so new/changed rows show. */
      this.props.dispatch(fetchChfSubscribers(this.params()));
      this.setState({
        document: { action: '', visible: false, dimmed: false },
      });
    },
    actions: {
      create: () => {
        this.documentHandler.show('create');
      },
      createPool: () => {
        this.documentHandler.show('create-pool');
      },
      update: bucketId => {
        this.documentHandler.show('update', { bucketId });
      },
    },
  };

  confirmHandler = {
    show: bucketId => {
      this.setState({ confirm: { visible: true, bucketId } });
    },
    hide: () => {
      this.setState({
        confirm: { ...this.state.confirm, visible: false },
      });
    },
    actions: {
      delete: () => {
        const { dispatch } = this.props;
        if (this.state.confirm.visible === true) {
          const { bucketId } = this.state.confirm;
          this.confirmHandler.hide();
          this.documentHandler.hide();
          if (isPoolId(bucketId))
            this.deletePool(bucketId);
          else
            dispatch(deleteChfSubscriber(bucketId));
        }
      },
    },
  };

  /*
   * A pool is deleted through the TMF API, which tells the CHF to end every
   * member's sessions on it.  The REST resource would delete the document
   * and leave those sessions drawing on nothing until their next report.
   */
  deletePool = async bucketId => {
    const { dispatch } = this.props;
    try {
      await deletePool(bucketId);
      dispatch(Notification.success({
        title: 'CHF Quota',
        message: `Shared pool ${bucketId} has been deleted`,
      }));
    } catch (error) {
      const { title, message } = poolError(error);
      dispatch(Notification.error({
        title, message, autoDismiss: 0, action: { label: 'Dismiss' },
      }));
    }
    dispatch(fetchChfSubscribers(this.params()));
  };

  render() {
    const { documentHandler, confirmHandler } = this;
    const { document } = this.state;
    const { status, pools } = this.props;
    const deletingPool = isPoolId(this.state.confirm.bucketId);

    return (
      <Layout.Content>
        {!pools && <ChfStats windowHours={1} />}
        <ConnectedSubscriberData
          active={this.isActive()}
          queryParams={this.params()}
          page={this.state.page}
          pageSize={PAGE_SIZE}
          search={this.state.searchInput}
          dispatch={this.props.dispatch}
          deletedSupi={status.id}
          onEdit={documentHandler.actions.update}
          onDelete={confirmHandler.show}
          onManagePool={pools ? undefined : this.handleManagePool}
          onCreatePool={documentHandler.actions.createPool}
          pools={pools}
          onBrowse={this.handleBrowse}
          onSearchChange={this.handleSearchChange}
          onPrev={this.handlePrev}
          onNext={this.handleNext}
        />
        {pools ? (
          <FloatingButton onClick={documentHandler.actions.createPool}
            icon={GroupAddIcon} title="New shared pool for several SIMs" />
        ) : (
          <FloatingButton onClick={documentHandler.actions.create}
            title="New quota profile for one SIM" />
        )}
        <Document
          {...document}
          onEdit={documentHandler.actions.update}
          onDelete={confirmHandler.show}
          onHide={documentHandler.hide}
        />
        <Dimmed visible={document.dimmed} />
        <Confirm
          visible={this.state.confirm.visible}
          message={deletingPool
            ? "Delete this shared pool? Every member's sessions on it are released."
            : 'Delete this CHF quota profile?'}
          onOutside={confirmHandler.hide}
          buttons={[
            { text: 'CANCEL', action: confirmHandler.hide, info: true },
            { text: 'DELETE', action: confirmHandler.actions.delete, danger: true },
          ]}
        />
      </Layout.Content>
    );
  }
}

Collection.propTypes = {
  dispatch: PropTypes.func.isRequired,
  status: PropTypes.object.isRequired,
  /* The CHF Shared Pools view. */
  pools: PropTypes.bool,
  openPoolId: PropTypes.string,
};

Collection = connect((state) => ({
  status: selectActionStatus(MODEL, state.crud, 'delete'),
  openPoolId: state.sidebar.openPool,
}))(Collection);

export default Collection;

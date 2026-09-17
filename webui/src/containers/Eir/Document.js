import { Component } from 'react';
import { connect } from 'react-redux';
import NProgress from 'nprogress';
import {
  MODEL,
  fetchEirRecords,
  fetchEirRecord,
  createEirRecord,
  updateEirRecord
} from 'modules/crud/eir';
import { clearActionStatus } from 'modules/crud/actions';
import { select, selectActionStatus } from 'modules/crud/selectors';
import * as Notification from 'modules/notification/actions';
import { Eir } from 'components';
const empty = { pei_type: 'imei', pei: '', imsi: '', status: 'WHITELISTED' };
const decode = (r) => {
  if (!r) return empty;
  const p = (r.pei || '').match(/^(imei|imeisv)-(\d+)$/) || [];
  const s = (r.supi || '').match(/^imsi-(\d+)$/) || [];
  return {
    _id: r._id,
    pei_type: p[1] || 'imei',
    pei: p[2] || '',
    imsi: s[1] || '',
    status: r.status
  };
};
const encode = (f) => {
  const data = { pei: `${f.pei_type}-${f.pei}`, status: f.status };
  if (f.imsi) data.supi = `imsi-${f.imsi}`;
  return data;
};
class Document extends Component {
  state = { formData: empty };
  componentWillMount() {
    if (this.props.record.needsFetch)
      this.props.dispatch(this.props.record.fetch);
  }
  componentWillReceiveProps(p) {
    if (p.record.needsFetch) p.dispatch(p.record.fetch);
    this.setState({ formData: decode(p.record.data) });
    if (p.status.response) {
      NProgress.done(true);
      p.dispatch(
        Notification.success({
          title: 'EIR',
          message:
            p.action === 'create'
              ? 'New EIR rule created'
              : `${p.status.id} EIR rule updated`
        })
      );
      p.dispatch(clearActionStatus(MODEL, p.action));
      p.onHide();
    }
    if (p.status.error) {
      NProgress.done(true);
      const response = (p.status.error || {}).response || {};
      p.dispatch(
        Notification.error({
          title: (response.data || {}).name || response.status || 'EIR Error',
          message:
            (response.data || {}).message ||
            response.statusText ||
            'Unable to save EIR rule',
          autoDismiss: 0,
          action: { label: 'Dismiss' }
        })
      );
      p.dispatch(clearActionStatus(MODEL, p.action));
    }
  }
  validate = (f, e) => {
    const length = f.pei_type === 'imeisv' ? 16 : 15;
    if (f.pei && f.pei.length !== length)
      e.pei.addError(
        `${f.pei_type.toUpperCase()} must contain ${length} digits`
      );
    if (f.imsi && (f.imsi.length < 6 || f.imsi.length > 15))
      e.imsi.addError('IMSI must contain 6 to 15 digits');
    if (this.props.action === 'create') {
      const data = encode(f);
      const records = this.props.records.data;
      if (records.some((r) => r.pei === data.pei))
        e.pei.addError('This equipment identity already exists');
      if (data.supi && records.some((r) => r.supi === data.supi))
        e.imsi.addError('This IMSI already exists');
    }
    return e;
  };
  submit = (f) => {
    NProgress.configure({ parent: '#nprogress-base-form', trickleSpeed: 5 });
    NProgress.start();
    const data = encode(f);
    if (this.props.action === 'create')
      this.props.dispatch(createEirRecord({}, data));
    else this.props.dispatch(updateEirRecord(f._id, {}, data));
  };
  handleError = (errors) =>
    errors.map((error) =>
      this.props.dispatch(
        Notification.error({ title: 'Validation Error', message: error.stack })
      )
    );
  render() {
    const p = this.props;
    return (
      <Eir.Edit
        visible={p.visible}
        action={p.action}
        formData={this.state.formData}
        isLoading={p.record.isLoading && !p.status.pending}
        validate={this.validate}
        onHide={p.onHide}
        onSubmit={this.submit}
        onError={this.handleError}
      />
    );
  }
}
export default connect((state, props) => ({
  records: select(fetchEirRecords(), state.crud),
  record: select(fetchEirRecord(props.id), state.crud),
  status: selectActionStatus(MODEL, state.crud, props.action)
}))(Document);

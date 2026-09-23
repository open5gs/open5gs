import { Component } from 'react';
import { connect } from 'react-redux';
import { MODEL, fetchEirRecords, deleteEirRecord } from 'modules/crud/eir';
import { clearActionStatus } from 'modules/crud/actions';
import { select, selectActionStatus } from 'modules/crud/selectors';
import * as Notification from 'modules/notification/actions';
import {
  Layout,
  Eir,
  Spinner,
  FloatingButton,
  Blank,
  Dimmed,
  Confirm
} from 'components';
import Document from './Document';
class Collection extends Component {
  state = {
    search: '',
    document: { action: '', visible: false, dimmed: false },
    confirm: { visible: false, id: '' },
    view: { visible: false, disableOnClickOutside: false, id: '' }
  };
  componentWillMount() {
    if (this.props.records.needsFetch)
      this.props.dispatch(this.props.records.fetch);
  }
  componentWillReceiveProps(nextProps) {
    const { records, status, dispatch } = nextProps;
    if (records.needsFetch) dispatch(records.fetch);
    if (status.response) {
      dispatch(
        Notification.success({
          title: 'EIR',
          message: `${status.id} has been deleted`
        })
      );
      dispatch(clearActionStatus(MODEL, 'delete'));
    }
  }
  handleSearchChange = (e) => this.setState({ search: e.target.value });
  handleSearchClear = () => this.setState({ search: '' });
  documentHandler = {
    show: (action, payload) =>
      this.setState({
        document: { action, visible: true, dimmed: true, ...payload },
        view: { ...this.state.view, disableOnClickOutside: true }
      }),
    hide: () =>
      this.setState({
        document: { action: '', visible: false, dimmed: false },
        view: { ...this.state.view, disableOnClickOutside: false }
      }),
    actions: {
      create: () => this.documentHandler.show('create'),
      update: (id) => this.documentHandler.show('update', { id })
    }
  };
  confirmHandler = {
    show: (id) =>
      this.setState({
        confirm: { visible: true, id },
        view: { ...this.state.view, disableOnClickOutside: true }
      }),
    hide: () =>
      this.setState({
        confirm: { ...this.state.confirm, visible: false },
        view: { ...this.state.view, disableOnClickOutside: false }
      }),
    actions: {
      delete: () => {
        if (this.state.confirm.visible) {
          const id = this.state.confirm.id;
          this.confirmHandler.hide();
          this.documentHandler.hide();
          this.viewHandler.hide();
          this.props.dispatch(deleteEirRecord(id));
        }
      }
    }
  };
  viewHandler = {
    show: (id) =>
      this.setState({
        view: { id, visible: true, disableOnClickOutside: false }
      }),
    hide: () => this.setState({ view: { ...this.state.view, visible: false } })
  };
  render() {
    const { search, document, view } = this.state;
    const { records, status } = this.props;
    const data = records.data;
    return (
      <Layout.Content>
        {data.length > 0 && (
          <Eir.Search
            value={search}
            onChange={this.handleSearchChange}
            onClear={this.handleSearchClear}
          />
        )}
        <Eir.List
          records={data}
          deletedId={status.id}
          onView={this.viewHandler.show}
          onEdit={this.documentHandler.actions.update}
          onDelete={this.confirmHandler.show}
          search={search}
        />
        {records.isLoading && <Spinner md />}
        <Blank
          visible={!records.isLoading && data.length === 0}
          title="ADD AN EIR RULE"
          body="You have no equipment identity rules... yet!"
          onTitle={this.documentHandler.actions.create}
        />
        <FloatingButton onClick={this.documentHandler.actions.create} />
        <Eir.View
          visible={view.visible}
          record={data.filter((record) => record._id === view.id)[0]}
          disableOnClickOutside={view.disableOnClickOutside}
          onEdit={this.documentHandler.actions.update}
          onDelete={this.confirmHandler.show}
          onHide={this.viewHandler.hide}
        />
        <Document {...document} onHide={this.documentHandler.hide} />
        <Dimmed visible={document.dimmed || view.visible} />
        <Confirm
          visible={this.state.confirm.visible}
          message="Delete this EIR rule?"
          onOutside={this.confirmHandler.hide}
          buttons={[
            { text: 'CANCEL', action: this.confirmHandler.hide, info: true },
            {
              text: 'DELETE',
              action: this.confirmHandler.actions.delete,
              danger: true
            }
          ]}
        />
      </Layout.Content>
    );
  }
}
export default connect((state) => ({
  records: select(fetchEirRecords(), state.crud),
  status: selectActionStatus(MODEL, state.crud, 'delete')
}))(Collection);

import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { MODEL, fetchAccounts, deleteAccount } from 'modules/crud/account';
import { clearActionStatus } from 'modules/crud/actions';
import { select, selectActionStatus } from 'modules/crud/selectors';
import * as Notification from 'modules/notification/actions';

import { 
  Layout, 
  Account, 
  Spinner, 
  FloatingButton, 
  Blank,
  Dimmed,
  Confirm
} from 'components';

import Document from './Document';

class Collection extends Component {
  state = {
    document: {
      action: '',
      visible: false,
      dimmed: false
    },
    confirm: {
      visible: false,
      username: ''
    },
  };

  componentWillMount() {
    const { accounts, dispatch } = this.props

    if (accounts.needsFetch) {
      dispatch(accounts.fetch)
    }
  }

  componentWillReceiveProps(nextProps) {
    const { accounts, status } = nextProps
    const { dispatch } = this.props

    if (accounts.needsFetch) {
      dispatch(accounts.fetch)
    }

    if (status.response) {
      dispatch(Notification.success({
        title: 'Account',
        message: `${status.id} has been deleted`
      }));
      dispatch(clearActionStatus(MODEL, 'delete'));
    } 

    if (status.error) {
      let title = 'Unknown Code';
      let message = 'Unknown Error';
      if (response.data && response.data.name && response.data.message) {
        title = response.data.name;
        message = response.data.message;
      } else {
        title = response.status;
        message = response.statusText;
      }

      dispatch(Notification.error({
        title,
        message,
        autoDismiss: 0,
        action: {
          label: 'Dismiss'
        }
      }));
      dispatch(clearActionStatus(MODEL, 'delete'));
    }
  }

  documentHandler = {
    show: (action, payload) => {
      this.setState({
        document: {
          action,
          visible: true,
          dimmed: true,
          ...payload
        }
      })
    },
    hide: () => {
      this.setState({
        document: {
          action: '',
          visible: false,
          dimmed: false
        },
      })
    },
    actions: {
      create: () => {
        this.documentHandler.show('create');
      },
      update: (username) => {
        this.documentHandler.show('update', { username });
      }
    }
  }

  confirmHandler = {
    show: (username) => {
      this.setState({
        confirm: {
          visible: true,
          username,
        }
      })
    },
    hide: () => {
      this.setState({
        confirm: {
          ...this.state.confirm,
          visible: false
        }
      })
    },
    actions : {
      delete: () => {
        const { dispatch } = this.props

        if (this.state.confirm.visible === true) {
          this.confirmHandler.hide();
          this.documentHandler.hide();

          dispatch(deleteAccount(this.state.confirm.username));
        }
      }
    }
  }

  render() {
    const {
      handleSearchChange,
      handleSearchClear,
      documentHandler,
      confirmHandler
    } = this;

    const { 
      document
    } = this.state;

    const { 
      session,
      accounts,
      status
    } = this.props

    const {
      username,
      roles
    } = session.user;

    const {
      isLoading,
      data
    } = accounts;

    return (
      <Layout.Content>
        <Account.List
          session={session}
          accounts={data}
          deletedId={status.id}
          onEdit={documentHandler.actions.update}
          onDelete={confirmHandler.show}
        />
        {isLoading && <Spinner md />}
        {roles.indexOf('admin') !== -1 && <FloatingButton onClick={documentHandler.actions.create}/>}
        <Document 
          { ...document }
          session={session}
          onEdit={documentHandler.actions.update}
          onDelete={confirmHandler.show}
          onHide={documentHandler.hide} />
        <Dimmed visible={document.dimmed} />
        <Confirm
          visible={this.state.confirm.visible}
          message="Delete this account?"
          onOutside={confirmHandler.hide}
          buttons={[
            { text: "CANCEL", action: confirmHandler.hide, info:true },
            { text: "DELETE", action: confirmHandler.actions.delete, danger:true }
          ]}/>
      </Layout.Content>
    )
  }
}

Collection = connect(
  (state) => ({ 
    accounts: select(fetchAccounts(), state.crud),
    status: selectActionStatus(MODEL, state.crud, 'delete')
  })
)(Collection);

export default Collection;

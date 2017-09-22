import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import NProgress from 'nprogress';

import { MODEL, fetchAccounts, fetchAccount, createAccount, updateAccount } from 'modules/crud/account';
import { clearActionStatus } from 'modules/crud/actions';
import { select, selectActionStatus } from 'modules/crud/selectors';
import * as Notification from 'modules/notification/actions';

import { Account } from 'components';

import traverse from 'traverse';

const formData = {
  "roles": [ "user" ]
}

class Document extends Component {
  static propTypes = {
    action: PropTypes.string,
    visible: PropTypes.bool, 
    onHide: PropTypes.func
  }

  state = {
    formData
  }

  componentWillMount() {
    const { account, dispatch } = this.props

    if (account.needsFetch) {
      dispatch(account.fetch)
    }
  }

  componentWillReceiveProps(nextProps) {
    const { account, status } = nextProps
    const { dispatch, action, onHide } = this.props

    if (account.needsFetch) {
      dispatch(account.fetch)
    }

    if (account.data) {
      this.setState({ formData: account.data })
    } else {
      this.setState({ formData });
    }

    if (status.response) {
      NProgress.configure({ 
        parent: 'body',
        trickleSpeed: 5
      });
      NProgress.done(true);

      const message = action === 'create' ? "New account created" : `${status.id} account updated`;

      dispatch(Notification.success({
        title: 'Account',
        message
      }));

      dispatch(clearActionStatus(MODEL, action));
      onHide();
    } 

    if (status.error) {
      NProgress.configure({ 
        parent: 'body',
        trickleSpeed: 5
      });
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

      dispatch(Notification.error({
        title,
        message,
        autoDismiss: 0,
        action: {
          label: 'Dismiss',
          callback: () => onHide()
        }
      }));
      dispatch(clearActionStatus(MODEL, action));
    }
  }

  validate = (formData, errors) => {
    const { accounts, action, status } = this.props;
    const { username } = formData;

    if (action === 'create' && accounts && accounts.data &&
      accounts.data.filter(account => account.username === username).length > 0) {
      errors.username.addError(`'${username}' is duplicated`);
    }

    return errors;
  }

  handleSubmit = (formData) => {
    const { dispatch, action } = this.props;

    NProgress.configure({ 
      parent: '#nprogress-base-form',
      trickleSpeed: 5
    });
    NProgress.start();

    if (action === 'create') {
      dispatch(createAccount({}, formData));
    } else if (action === 'update') {
      dispatch(updateAccount(formData.username, {}, formData));
    } else {
      throw new Error(`Action type '${action}' is invalid.`);
    }
  }

  handleError = errors => {
    const { dispatch } = this.props;
    errors.map(error =>
      dispatch(Notification.error({
        title: 'Validation Error',
        message: error.stack
      }))
    )
  }

  render() {
    const {
      validate,
      handleSubmit,
      handleError
    } = this;

    const { 
      visible,
      action,
      status,
      account,
      onHide
    } = this.props

    return (
      <Account.Edit
        visible={visible} 
        action={action}
        formData={this.state.formData}
        isLoading={account.isLoading && !status.pending}
        validate={validate}
        onHide={onHide}
        onSubmit={handleSubmit}
        onError={handleError} />
    )
  }
}

Document = connect(
  (state, props) => ({ 
    accounts: select(fetchAccounts(), state.crud),
    account: select(fetchAccount(props.username), state.crud),
    status: selectActionStatus(MODEL, state.crud, props.action)
  })
)(Document);

export default Document;

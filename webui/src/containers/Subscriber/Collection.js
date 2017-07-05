import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { MODEL, fetchSubscribers, deleteSubscriber } from 'modules/crud/subscriber';
import { clearActionStatus } from 'modules/crud/actions';
import { select, selectActionStatus } from 'modules/crud/selectors';
import * as Notification from 'modules/notification/actions';

import { 
  Layout, 
  Subscriber, 
  Spinner, 
  FloatingButton, 
  Blank,
  Dimmed
} from 'components';

import Document from './Document';

class Collection extends Component {
  state = {
    search: '',
    document: {
      action: '',
      visible: false,
      dimmed: false
    }
  };

  componentWillMount() {
    const { subscribers, dispatch } = this.props

    if (subscribers.needsFetch) {
      dispatch(subscribers.fetch)
    }
  }

  componentWillReceiveProps(nextProps) {
    const { subscribers, status } = nextProps
    const { dispatch } = this.props

    if (subscribers.needsFetch) {
      dispatch(subscribers.fetch)
    }

    if (status.response) {
      dispatch(Notification.success({
        title: 'Subscriber',
        message: `${status.id} has been deleted`
      }));
      dispatch(clearActionStatus(MODEL, 'delete'));
    } 

    if (status.error) {
      const title = ((((status || {}).error || {}).response || {}).data || {}).name || 'Server Error';
      const message = ((((status || {}).error || {}).response || {}).data || {}).message || 'Unknown Error';

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

  handleSearchChange = (e) => {
    this.setState({
      search: e.target.value
    });
  }

  handleSearchClear = (e) => {
    this.setState({
      search: ''
    });
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
          ...this.state.document,
          visible: false,
          dimmed: false
        }
      })
    },
    actions: {
      create: () => {
        this.documentHandler.show('create');
      },
      browser: (imsi) => {
      },
      update: (imsi) => {
        this.documentHandler.show('update', { imsi });
      },
      delete: (imsi) => {
        const { dispatch } = this.props
        dispatch(deleteSubscriber(imsi));
      }
    }
  }

  render() {
    const {
      handleSearchChange,
      handleSearchClear,
      documentHandler
    } = this;

    const { 
      search,
      document
    } = this.state;

    const { 
      subscribers
    } = this.props

    const {
      isLoading,
      data
    } = subscribers;

    return (
      <Layout.Content>
        {Object.keys(data).length > 0 && <Subscriber.Search 
          onChange={handleSearchChange}
          value={search}
          onClear={handleSearchClear} />}
        <Subscriber.List
          subscribers={data}
          onShow={documentHandler.actions.browser}
          onEdit={documentHandler.actions.update}
          onDelete={documentHandler.actions.delete}
          search={search}
        />
        {isLoading && <Spinner md />}
        <Blank
          visible={!isLoading && !(Object.keys(data).length > 0)}
          title="ADD A SUBSCRIBER"
          body="You have no subscribers... yet!"
          onTitle={documentHandler.actions.create}
          />
        <FloatingButton onClick={documentHandler.actions.create}/>
        <Document 
          { ...document }
          onHide={documentHandler.hide} />
        <Dimmed visible={document.dimmed} />
      </Layout.Content>
    )
  }
}

Collection = connect(
  (state) => ({ 
    subscribers: select(fetchSubscribers(), state.crud),
    status: selectActionStatus(MODEL, state.crud, 'delete')
  })
)(Collection);

export default Collection;
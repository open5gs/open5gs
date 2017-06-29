import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { fetchSubscribers, deleteSubscriber } from 'modules/crud/subscriber';
import { select } from 'modules/crud/selectors';

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
    const { subscribers } = nextProps
    const { dispatch } = this.props
    if (subscribers.needsFetch) {
      dispatch(subscribers.fetch)
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
      add: () => {
        this.documentHandler.show('add');
      },
      browser: (imsi) => {
      },
      change: (imsi) => {
        this.documentHandler.show('change', { imsi });
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
        {data.length !== 0 && <Subscriber.Search 
          onChange={handleSearchChange}
          value={search}
          onClear={handleSearchClear} />}
        <Subscriber.List
          subscribers={data}
          onShow={documentHandler.actions.browser}
          onEdit={documentHandler.actions.change}
          onDelete={documentHandler.actions.delete}
          search={search}
        />
        {isLoading && <Spinner md />}
        <Blank
          visible={!isLoading && !data.length}
          title="ADD A SUBSCRIBER"
          body="You have no subscribers... yet!"
          onTitle={documentHandler.actions.add}
          />
        <FloatingButton onClick={documentHandler.actions.add}/>
        <Document 
          { ...document }
          onHide={documentHandler.hide} />
        <Dimmed visible={document.dimmed} />
      </Layout.Content>
    )
  }
}

function mapStateToProps(state) {
  return { subscribers: select(fetchSubscribers(), state.crud) }
}


Collection = connect(
  (state) => ({ 
    subscribers: select(fetchSubscribers(), state.crud)
  })
)(Collection);

export default Collection;
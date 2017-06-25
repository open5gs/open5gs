import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { fetchSubscribers } from 'modules/crud/subscriber';
import { select } from 'modules/crud/selectors';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import { 
  Layout, 
  Subscriber, 
  Spinner, 
  FloatingButton, 
  Blank,
  Dimmed
} from 'components';

class SubscriberContainer extends Component {
  state = {
    search: '',
    form: {
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

  formHandler = {
    show: () => {
      this.setState({
        form: {
          visible: true,
          dimmed: true
        }
      })
    },
    hide: () => {
      this.setState({
        form: {
          visible: false,
          dimmed: false
        }
      })
    },
    submit: async () => {
      this.setState({
        form: {
          visible: false,
          dimmed: false
        }
      })
    }
  }

  handleShow = (imsi) => {
  }

  handleEdit = (imsi) => {
  }

  handleDelete = (imsi) => {
  }

  render() {
    const {
      handleSearchChange,
      handleSearchClear,
      formHandler,
      handleShow,
      handleEdit,
      handleDelete
    } = this;

    const { 
      search,
      form
    } = this.state;

    const { 
      subscribers 
    } = this.props

    const {
      isLoading,
      data
    } = subscribers;

    const {
      length
    } = data;

    return (
      <Layout.Content>
        {length !== 0 && <Subscriber.Search 
          onChange={handleSearchChange}
          value={search}
          onClear={handleSearchClear} />}
        <Subscriber.List
          subscribers={subscribers.data}
          onShow={handleShow}
          onEdit={handleEdit}
          onDelete={handleDelete}
          search={search}
        />
        {isLoading && <Spinner md color={oc.indigo[9]} />}
        <Blank
          visible={!isLoading && !length}
          title="ADD A SUBSCRIBER"
          body="You have no subscribers... yet!"
          onTitle={formHandler.show}
          />
        <FloatingButton onClick={formHandler.show}/>
        <Subscriber.Form
          visible={form.visible} 
          onHide={formHandler.hide}
          onSubmit={formHandler.submit} />
        <Dimmed visible={form.dimmed} />
      </Layout.Content>
    )
  }
}

function mapStateToProps(state, ownProps) {
  return { subscribers: select(fetchSubscribers(), state.crud) }
}

export default connect(mapStateToProps)(SubscriberContainer)
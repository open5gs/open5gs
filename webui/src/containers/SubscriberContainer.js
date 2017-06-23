import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { fetchSubscribers } from 'modules/crud/subscriber';
import { select } from 'modules/crud/selectors';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import { Layout, Subscriber, Spinner, FloatingButton, Blank } from 'components';

class SubscriberContainer extends Component {
  state = {
    search: ''
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

  handleAdd = (e) => {
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
      handleAdd,
      handleShow,
      handleEdit,
      handleDelete
    } = this;

    const { 
      search
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
          onTitle={handleAdd}
          />
        <FloatingButton onClick={handleAdd}/>
      </Layout.Content>
    )
  }
}

function mapStateToProps(state, ownProps) {
  return { subscribers: select(fetchSubscribers(), state.crud) }
}

export default connect(mapStateToProps)(SubscriberContainer)
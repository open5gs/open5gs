import { Component } from 'react';
import PropTypes from 'prop-types';

import { connect } from 'react-redux'

import { fetchSubscribers } from 'actions/subscriber';
import { select } from 'redux-crud-store'

class SubscriberContainer extends Component {
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

  render() {
    const { subscribers } = this.props
    if (subscribers.isLoading) {
      return <div>
        <p>loading...</p>
      </div>
    } else {
      return <div>
        <p>success...</p>
      </div>
    }
  }
}

function mapStateToProps(state, ownProps) {
  return { subscribers: select(fetchSubscribers(), state.db) }
}

export default connect(mapStateToProps)(SubscriberContainer)

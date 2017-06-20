import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { fetchSubscribers } from 'modules/crud/subscriber';
import { select } from 'modules/crud/selectors';

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
        { subscribers.data.map(subscriber => 
          <li key={subscriber.imsi}>{subscriber.imsi}{subscriber.pdn[0].apn}</li>
          )
        }
      </div>
    }
  }
}

function mapStateToProps(state, ownProps) {
  return { subscribers: select(fetchSubscribers(), state.crud) }
}

export default connect(mapStateToProps)(SubscriberContainer)
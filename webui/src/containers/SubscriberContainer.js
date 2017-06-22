import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { fetchSubscribers } from 'modules/crud/subscriber';
import { select } from 'modules/crud/selectors';

import oc from 'open-color';
import { Spinner, Subscriber } from 'components';

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

    return (
      <div>
        <Subscriber.Search />
        <Subscriber.List subscribers={subscribers.data} />
        {subscribers.isLoading && <Spinner md color={oc.indigo[9]} />}
      </div>
    )
  }
}

function mapStateToProps(state, ownProps) {
  return { subscribers: select(fetchSubscribers(), state.crud) }
}

export default connect(mapStateToProps)(SubscriberContainer)
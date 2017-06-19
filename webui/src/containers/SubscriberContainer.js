import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { fetchSubscribers } from 'actions/subscriber';
import { fetchCollection } from 'actions/crud';

class SubscriberContainer extends Component {
  componentWillMount() {
    const { crud, dispatch } = this.props
      dispatch(fetchSubscribers());
  }

  render() {
      return <div>
        <p>loading...</p>
      </div>
  }
}

export default connect(
  (state) => ({
    crud: state.crud
  })
)(SubscriberContainer);

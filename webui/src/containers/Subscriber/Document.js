import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { fetchSubscriber, fetchSubscribers } from 'modules/crud/subscriber';
import { select } from 'modules/crud/selectors';

import { 
  Subscriber, 
  Spinner
} from 'components';

class Document extends Component {
  componentWillMount() {
    const { subscriber, dispatch } = this.props

    if (subscriber.needsFetch) {
      dispatch(subscriber.fetch)
    }
  }

  componentWillReceiveProps(nextProps) {
    const { subscriber } = nextProps
    const { dispatch } = this.props
    if (subscriber.needsFetch) {
      dispatch(subscriber.fetch)
    }
  }

  handleSubmit = () => {

  }

  render() {
    const {
      handleSubmit
    } = this;

    const { 
      visible,
      action,
      subscribers,
      subscriber,
      onHide
    } = this.props

    const {
      isLoading,
      data
    } = subscriber;

    const title = (action === 'change') ? 'Edit Subscriber' : 'Create Subscriber';

    return (
      <div>
        {isLoading && <Spinner md/>}
        <Subscriber.Edit
          formData={data}
          title={title}
          visible={visible} 
          onHide={onHide}
          onSubmit={handleSubmit} />
      </div>
    )
  }
}

function mapStateToProps(state, ownProps) {
  let subscriber = {};
  if (ownProps.imsi) {
    subscriber = select(fetchSubscriber(ownProps.imsi), state.crud);
  }

  return { 
    subscriber: subscriber
  }
}

export default connect(mapStateToProps)(Document)
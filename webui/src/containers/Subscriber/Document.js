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
  static propTypes = {
    action: PropTypes.string,
    visible: PropTypes.bool, 
    onHide: PropTypes.func, 
    onSubmit: PropTypes.func,
  }

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

  validate = (formData, errors) => {
    const { subscribers, action } = this.props;
    const { imsi } = formData;
    
    if (action === 'add' && subscribers && subscribers.data &&
      subscribers.data.filter(subscriber => subscriber.imsi === imsi).length > 0) {
      errors.imsi.addError(`'${imsi}' is duplicated`);
    }

    return errors;
  }

  handleSubmit = (formData) => {
    console.log(formData);
  }

  render() {
    const {
      validate,
      handleSubmit
    } = this;

    const { 
      action,
      visible,
      subscriber,
      onHide
    } = this.props

    const {
      isLoading,
      data
    } = subscriber;

    return (
      <div>
        {isLoading && <Spinner/>}
        <Subscriber.Edit
          visible={visible} 
          title={(action === 'change') ? 'Edit Subscriber' : 'Create Subscriber'}
          formData={data}
          validate={validate}
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
    subscribers: select(fetchSubscribers(), state.crud),
    subscriber: subscriber
  }
}

export default connect(mapStateToProps)(Document)
import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { 
  fetchSubscribers,
  fetchSubscriber, 
  createSubscriber,
  updateSubscriber
} from 'modules/crud/subscriber';

import {
  clearActionStatus
} from 'modules/crud/actions';

import { 
  select, 
  selectActionStatus 
} from 'modules/crud/selectors';

import { Subscriber } from 'components';

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
    const { subscriber, status } = nextProps
    const { dispatch, action, onHide } = this.props
    if (subscriber.needsFetch) {
      dispatch(subscriber.fetch)
    }

    if (status.response) {
      dispatch(clearActionStatus('subscribers', action));
      onHide();
    }
  }

  validate = (formData, errors) => {
    const { subscribers, action } = this.props;
    const { imsi } = formData;
    
    if (action === 'create' && subscribers && subscribers.data &&
      subscribers.data.filter(subscriber => subscriber.imsi === imsi).length > 0) {
      errors.imsi.addError(`'${imsi}' is duplicated`);
    }

    return errors;
  }

  handleSubmit = (formData) => {
    const { dispatch, action } = this.props;
    if (action === 'create') {
      dispatch(createSubscriber({}, formData));
    } else if (action === 'update') {
      dispatch(updateSubscriber(formData.imsi, {}, formData));
    } else {
      throw new Error(`Action type '${action}' is invalid.`);
    }
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
      <Subscriber.Edit
        visible={visible} 
        action={action}
        isLoading={isLoading}
        formData={data}
        validate={validate}
        onHide={onHide}
        onSubmit={handleSubmit} />
    )
  }
}

Document = connect(
  (state, props) => ({ 
    subscribers: select(fetchSubscribers(), state.crud),
    subscriber: select(fetchSubscriber(props.imsi), state.crud),
    status: selectActionStatus('subscribers', state.crud, props.action)
  })
)(Document);

export default Document;
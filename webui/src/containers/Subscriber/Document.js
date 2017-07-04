import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import NProgress from 'nprogress';

import { MODEL, fetchSubscribers, fetchSubscriber, createSubscriber, updateSubscriber } from 'modules/crud/subscriber';
import { clearActionStatus } from 'modules/crud/actions';
import { select, selectActionStatus } from 'modules/crud/selectors';
import * as Notification from 'modules/notification/actions';

import { Subscriber } from 'components';

const formData = {
  "security": {
    k: "465B5CE8 B199B49F AA5F0A2E E238A6BC",
    op: "5F1D289C 5D354D0A 140C2548 F5F3E3BA",
    amf: "8000"
  },
  "ue_ambr": {
    "max_bandwidth_ul": 1024000,
    "max_bandwidth_dl": 1024000
  },
  "pdn": [
    {
      "apn": "internet",
      "qos": {
        "qci": 9,
        "arp": {
          "priority_level": 8
        }
      },
      "pdn_ambr": {
        "max_bandwidth_ul": 1024000,
        "max_bandwidth_dl": 1024000
      }
    }
  ]
}

class Document extends Component {
  static propTypes = {
    action: PropTypes.string,
    visible: PropTypes.bool, 
    onHide: PropTypes.func, 
  }

  state = {
    formData,
    disableSubmitButton: true,
    disableValidation: false
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

    if (this.props.visible != nextProps.visible) {
      if (subscriber.data) {
        this.setState({ formData: subscriber.data })
      } else {
        this.setState({ formData });
      }
    }

    if (status.response) {
      NProgress.configure({ 
        parent: 'body',
        trickleSpeed: 5
      });
      NProgress.done(true);

      const message = action === 'create' ? "New subscriber created" : `${status.id} subscriber updated`;

      dispatch(Notification.success({
        title: 'Subscriber',
        message
      }));
      dispatch(clearActionStatus(MODEL, action));
      onHide();
    } 

    if (status.error) {
      NProgress.configure({ 
        parent: 'body',
        trickleSpeed: 5
      });
      NProgress.done(true);

      const title = ((((status || {}).error || {}).response || {}).data || {}).name || 'System Error';
      const message = ((((status || {}).error || {}).response || {}).data || {}).message || 'Unknown Error';

      dispatch(Notification.error({
        title,
        message,
        autoDismiss: 0,
        action: {
          label: 'Dismiss',
          callback: () => onHide()
        }
      }));
      dispatch(clearActionStatus(MODEL, action));
    }
  }

  validate = (formData, errors) => {
    const { subscribers, action, status } = this.props;
    const { disableValidation } = this.state;
    const { imsi } = formData;

    if (action === 'create' && disableValidation !== true && 
      subscribers && subscribers.data &&
      subscribers.data.filter(subscriber => subscriber.imsi === imsi).length > 0) {
      errors.imsi.addError(`'${imsi}' is duplicated`);
    }

    if (formData.pdn) {
      let apns = formData.pdn.map(pdn => { return pdn.apn } )
      let duplicates = {};
      for (let i = 0; i < apns.length; i++) {
        if (duplicates.hasOwnProperty(apns[i])) {
          duplicates[apns[i]].push(i);
        } else if (apns.lastIndexOf(apns[i]) !== i) {
          duplicates[apns[i]] = [i];
        }
      }

      for (let key in duplicates) {
        duplicates[key].forEach(index => 
          errors.pdn[index].apn.addError(`'${key}' is duplicated`));
      }
    }

    return errors;
  }

  handleChange = (formData, errors) => {
    let disableSubmitButton = (Object.keys(errors).length > 0);

    // I think there is a bug in React or Jsonschema library
    // For workaround, I'll simply add 'formData' in setState
    this.setState({
      disableSubmitButton,
      formData
    });
  }

  handleSubmit = (formData) => {
    const { dispatch, action } = this.props;

    this.setState({ disableValidation: true })

    NProgress.configure({ 
      parent: '#nprogress-base-form',
      trickleSpeed: 5
    });
    NProgress.start();

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
      handleChange,
      handleSubmit
    } = this;

    const { 
      visible,
      action,
      status,
      subscriber,
      onHide
    } = this.props

    return (
      <Subscriber.Edit
        visible={visible} 
        action={action}
        formData={this.state.formData}
        isLoading={subscriber.isLoading && !status.pending}
        disableSubmitButton={this.state.disableSubmitButton}
        validate={validate}
        onHide={onHide}
        onChange={handleChange}
        onSubmit={handleSubmit} />
    )
  }
}

Document = connect(
  (state, props) => ({ 
    subscribers: select(fetchSubscribers(), state.crud),
    subscriber: select(fetchSubscriber(props.imsi), state.crud),
    status: selectActionStatus(MODEL, state.crud, props.action)
  })
)(Document);

export default Document;
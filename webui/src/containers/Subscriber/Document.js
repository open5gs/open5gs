import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import NProgress from 'nprogress';

import { MODEL, fetchSubscribers, fetchSubscriber, createSubscriber, updateSubscriber } from 'modules/crud/subscriber';
import { fetchProfiles } from 'modules/crud/profile';
import { clearActionStatus } from 'modules/crud/actions';
import { select, selectActionStatus } from 'modules/crud/selectors';
import * as Notification from 'modules/notification/actions';

import { Subscriber } from 'components';

import traverse from 'traverse';

const formData = {
  "security": {
    k: "465B5CE8 B199B49F AA5F0A2E E238A6BC",
    amf: "8000",
    op_value: "E8ED289D EBA952E4 283B54E8 8E6183CA",
  },
  "ambr": {
    "downlink": 1024000,
    "uplink": 1024000
  },
  "pdn": [
    {
      "apn": "internet",
      "type": 2,
      "ambr": {
        "downlink": 1024000,
        "uplink": 1024000
      },
      "qos": {
        "qci": 9,
      // Ch 7.3.40 Allocation-Retenion-Proirty in TS 29.272 V15.9.0
      //
      // If the Pre-emption-Capability AVP is not present
      // in the Allocation-Retention-Priority AVP, the default value shall be
      // PRE-EMPTION_CAPABILITY_DISABLED (1).
      //
      // If the Pre-emption-Vulnerability AVP is not present
      // in the Allocation-Retention-Priority AVP, the default value shall be
      // PRE-EMPTION_VULNERABILITY_ENABLED (0).
      //
      // However, to easily set up VoLTE service,
      // disable Pre-emption Capability/Vulnerablility in Default Bearer
        "arp": {
          "priority_level": 8,
          "pre_emption_capability": 1,
          "pre_emption_vulnerability": 1
        }
      },
    }
  ]
}

class Document extends Component {
  static propTypes = {
    action: PropTypes.string,
    visible: PropTypes.bool, 
    onHide: PropTypes.func
  }

  state = {
    formData
  }

  componentWillMount() {
    const { subscriber, profiles, dispatch } = this.props

    if (subscriber.needsFetch) {
      dispatch(subscriber.fetch)
    }
    if (profiles.needsFetch) {
      dispatch(profiles.fetch)
    }
  }

  componentWillReceiveProps(nextProps) {
    const { subscriber, profiles, status } = nextProps
    const { dispatch, action, onHide } = this.props

    if (subscriber.needsFetch) {
      dispatch(subscriber.fetch)
    }
    if (profiles.needsFetch) {
      dispatch(profiles.fetch)
    }

    if (subscriber.data) {
      // Mongoose library has a problem for 64bit-long type
      //
      //   FETCH : the library returns 'Number' type for 64bit-long type
      //   CREATE/UPDATE : the library returns 'String' type for 64bit-long type
      //
      // In this case, I cannot avoid json-schema validation function
      // So, I've changed the type from 'String' to 'Number' if the key name is 'downlink' and 'uplink'
      // 
      //    The followings are changed from 'String' to 'Number' after DB CREATE or UPDATE
      //     - ambr.downlink, ambr.uplink, qos.mbr.downlink, qos.mbr.uplink, qos.gbr.downlink, qos.gbr.uplink
      // 
      traverse(subscriber.data).forEach(function(x) {
        if (this.key == 'downlink') this.update(Number(x));
        if (this.key == 'uplink') this.update(Number(x));
      })

      if (subscriber.data.security) {
        if (subscriber.data.security.opc) {
          subscriber.data.security.op_type = 0;
          subscriber.data.security.op_value = subscriber.data.security.opc;
        } else {
          subscriber.data.security.op_type = 1;
          subscriber.data.security.op_value = subscriber.data.security.op;
        }
      }

      this.setState({ formData: subscriber.data })
    } else {
      this.setState({ formData });
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

      const response = ((status || {}).error || {}).response || {};

      let title = 'Unknown Code';
      let message = 'Unknown Error';
      if (response.data && response.data.name && response.data.message) {
        title = response.data.name;
        message = response.data.message;
      } else {
        title = response.status;
        message = response.statusText;
      }

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
    const { imsi } = formData;

    if (action === 'create' && subscribers && subscribers.data &&
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

  handleSubmit = (formData) => {
    const { dispatch, action } = this.props;
    if (formData.security) {
      if (formData.security.op_type === 1) {
        formData.security.op = formData.security.op_value;
        formData.security.opc = null;
      } else {
        formData.security.op = null;
        formData.security.opc = formData.security.op_value;
      }
    }

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

  handleError = errors => {
    const { dispatch } = this.props;
    errors.map(error =>
      dispatch(Notification.error({
        title: 'Validation Error',
        message: error.stack
      }))
    )
  }

  render() {
    const {
      validate,
      handleSubmit,
      handleError
    } = this;

    const { 
      visible,
      action,
      status,
      subscriber,
      profiles,
      onHide
    } = this.props

    return (
      <Subscriber.Edit
        visible={visible} 
        action={action}
        formData={this.state.formData}
        profiles={profiles.data}
        isLoading={subscriber.isLoading && !status.pending}
        validate={validate}
        onHide={onHide}
        onSubmit={handleSubmit}
        onError={handleError} />
    )
  }
}

Document = connect(
  (state, props) => ({ 
    subscribers: select(fetchSubscribers(), state.crud),
    subscriber: select(fetchSubscriber(props.imsi), state.crud),
    profiles: select(fetchProfiles(), state.crud),
    status: selectActionStatus(MODEL, state.crud, props.action)
  })
)(Document);

export default Document;

import { Component } from 'react';
import PropTypes from 'prop-types';

import withWidth, { SMALL } from 'helpers/with-width';
import { Form } from 'components';

const schema = {
  "title": "Subscriber",
  "type": "object",
  "properties": {
    "imsi": {
      "type": "string", 
      "title": "IMSI*",
      "required": true,
      "pattern": "^\\d+$",
      "maxLength": 15,
      "messages": {
        "pattern": "Only digits are allowed"
      }
    },
    "security": {
      "title": "",
      "type": "object",
      "properties": {
        "k": {
          "type": "string",
          "title": "Subscriber Key (K)*",
          "required": true,
          "pattern": "^[0-9a-fA-F\\s]+$",
          "messages": {
            "pattern": "Only hexadecimal digits are allowed"
          }
        },
        "op": {
          "type": "string",
          "title": "Operator Key (OP)*",
          "required": true,
          "pattern": "^[0-9a-fA-F\\s]+$",
          "messages": {
            "pattern": "Only hexadecimal digits are allowed"
          }
        },
        "amf": {
          "type": "string",
          "title": "Authentication Management Field (AMF)*",
          "required": true,
          "pattern": "^[0-9a-fA-F\\s]+$",
          "messages": {
            "pattern": "Only hexadecimal digits are allowed"
          }
        }
      }
    },
    "ue_ambr": {
      "type": "object",
      "title": "",
      "properties": {
        "max_bandwidth_dl": {
          "type": "number",
          "title": "UE-AMBR Downlink (Kbps)*",
          "required": true
        },
        "max_bandwidth_ul": {
          "type": "number",
          "title": "UE-AMBR Uplink (Kbps)*",
          "required": true
        }
      }
    },
    "pdn": {
      "type": "array",
      "title": "EPS Session",
      "minItems": 1,
      "maxItems": 8,
      "messages": {
        "minItems": "At least 1 PDN is required",
        "maxItems": "8 PDNs are supported"
      },
      "items": {
        "type": "object",
        "properties": {
          "apn": {
            "type": "string",
            "title": "Access Point Name (APN)*",
            "required": true
          },
          "qos": {
            "type": "object",
            "title": "",
            "properties": {
              "qci": {
                "type": "number",
                "title": "QoS Class Identifier (QCI)*",
                "enum": [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 65, 66, 69, 70 ],
                "default": 9
              },
              "arp" : {
                "type": "object",
                "title": "",
                "properties": {
                  "priority_level": {
                    "type": "number",
                    "title": "ARP Priority Level (1-15)*",
                    "default": 8,
                    "minimum": 1,
                    "maximum": 15,
                    "required": true
                  },
                  "pre_emption_capability": {
                    "type": "number",
                    "title": "Capability*",
                    "enum": [1, 0],
                    "enumNames": ["Disabled", "Enabled"],
                    "default": 1,
                    "required": true
                  },
                  "pre_emption_vulnerability": {
                    "type": "number",
                    "title": "Vulnerability*",
                    "default": 1,
                    "minimum": 0,
                    "maximum": 1,
                    "enum": [1, 0],
                    "enumNames": ["Disabled", "Enabled"],
                    "default": 1,
                    "required": true
                  },
                }
              }
            }
          },
          "pdn_ambr": {
            "type": "object",
            "title": "",
            "properties": {
              "max_bandwidth_dl": {
                "type": "number",
                "title": "APN-AMBR Downlink (Kbps)",
              },
              "max_bandwidth_ul": {
                "type": "number",
                "title": "APN-AMBR Uplink (Kbps)",
              },
            }
          }
        }
      }
    }
  }
};

const uiSchema = {
  "imsi" : {
    classNames: "col-xs-12",
  },
  "security" : {
    "k" : {
      classNames: "col-xs-12",
    },
    "op" : {
      classNames: "col-xs-7",
    },
    "amf" : {
      classNames: "col-xs-5",
    },
  },
  "ue_ambr" : {
    "max_bandwidth_dl" : {
      classNames: "col-xs-6"
    },
    "max_bandwidth_ul" : {
      classNames: "col-xs-6"
    },
  },
  "pdn": {
    "ui:options":  {
      "orderable": false
    },
    "items": {
      "qos": {
        "qci": {
          "ui:widget": "radio",
          "ui:options": {
            "inline": true
          },
        },
        "arp": {
          "priority_level": {
            classNames: "col-xs-6"
          },
          "pre_emption_capability": {
            classNames: "col-xs-3"
          },
          "pre_emption_vulnerability": {
            classNames: "col-xs-3"
          }
        }
      },
      "pdn_ambr" : {
        "max_bandwidth_dl" : {
          classNames: "col-xs-6"
        },
        "max_bandwidth_ul" : {
          classNames: "col-xs-6"
        },
      }
    }
  }
}

class Edit extends Component {
  static propTypes = {
    visible: PropTypes.bool, 
    action: PropTypes.string, 
    formData: PropTypes.object,
    isLoading: PropTypes.bool,
    disabled: PropTypes.bool,
    disableSubmitButton: PropTypes.bool,
    validate: PropTypes.func, 
    onHide: PropTypes.func, 
    onChange: PropTypes.func,
    onSubmit: PropTypes.func,
    onError: PropTypes.func
  }

  constructor(props) {
    super(props);

    this.state = this.getStateFromProps(props);
  }

  componentWillMount() {
    this.setState(this.getStateFromProps(this.props));
  }

  componentWillReceiveProps(nextProps) {
    this.setState(this.getStateFromProps(nextProps));
  }

  getStateFromProps(props) {
    const { 
      action,
      width,
    } = props;

    let state = {
      schema,
      uiSchema
    };
    
    if (action === 'update') {
      state = {
        ...state,
        uiSchema : {
          ...uiSchema,
          "imsi": {
            "ui:disabled": true
          }
        }
      }
    } else if (width !== SMALL) {
      state = {
        ...state,
        uiSchema : {
          ...uiSchema,
          "imsi": {
            "ui:autofocus": true
          }
        }
      }
    }

    return state;
  }

  render() {
    const {
      visible,
      action,
      formData,
      disabled,
      isLoading,
      disableSubmitButton,
      validate,
      onHide,
      onChange,
      onSubmit,
      onError
    } = this.props;

    return (
      <Form 
        visible={visible}
        title={(action === 'update') ? 'Edit Subscriber' : 'Create Subscriber'}
        schema={this.state.schema}
        uiSchema={this.state.uiSchema}
        formData={formData}
        disabled={disabled}
        isLoading={isLoading}
        disableSubmitButton={disableSubmitButton}
        validate={validate}
        onHide={onHide}
        onChange={onChange}
        onSubmit={onSubmit}
        onError={onError}/>
    )
  }
}

export default withWidth()(Edit);
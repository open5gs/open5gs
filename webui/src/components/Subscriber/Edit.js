import { Component } from 'react';
import PropTypes from 'prop-types';

import withWidth, { SMALL } from 'helpers/with-width';
import { Form } from 'components';

const schema = {
  "title": "",
  "type": "object",
  "properties": {
    "imsi": {
      "type": "string", 
      "title": "IMSI (International Mobile Subscriber Identity)",
      "required": true,
      "pattern": "^\\d+$",
      "maxLength": 15,
      "messages": {
        "pattern": "Only digits are allowed"
      }
    },
    "security": {
      "title": "Security",
      "type": "object",
      "properties": {
        "k": {
          "type": "string",
          "title": "K (UE Key)",
          "required": true,
          "pattern": "^[0-9a-fA-F\\s]+$",
          "messages": {
            "pattern": "Only hexadecimal digits are allowed"
          }
        },
        "op": {
          "type": "string",
          "title": "OP (Operator Key)",
          "required": true,
          "pattern": "^[0-9a-fA-F\\s]+$",
          "messages": {
            "pattern": "Only hexadecimal digits are allowed"
          }
        },
        "amf": {
          "type": "string",
          "title": "AMF (Authentication Management Field)",
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
      "title": "UE AMBR - Aggregate Maximum Bit Rate",
      "properties": {
        "max_bandwidth_ul": {
          "type": "number",
          "title": "Max Requested Bandwidth UL (Kbps)",
          "required": true
        },
        "max_bandwidth_dl": {
          "type": "number",
          "title": "Max Requested Bandwidth DL (Kbps)",
          "required": true
        }
      }
    },
    "pdn": {
      "type": "array",
      "title": "PDN - Packet Data Network",
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
            "title": "APN (Access Point Name)",
            "required": true
          },
          "qos": {
            "type": "object",
            "title": "EPS Subscribed QoS Profile",
            "properties": {
              "qci": {
                "type": "number",
                "title": "QCI (QoS Class Identifier)",
                "enum": [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 65, 66, 69, 70 ],
                "default": 9
              },
              "arp" : {
                "type": "object",
                "title": "",
                "properties": {
                  "priority_level": {
                    "type": "number",
                    "title": "ARP Priority Level (1~15)",
                    "default": 8,
                    "minimum": 1,
                    "maximum": 15,
                    "required": true
                  }
                }
              }
            }
          },
          "pdn_ambr": {
            "type": "object",
            "title": "APN AMBR - Aggragate Maximum Bit Rate",
            "properties": {
              "max_bandwidth_ul": {
                "type": "number",
                "title": "Max Requested Bandwidth UL (Kbps)",
                "default": 1024000,
                "required": true
              },
              "max_bandwidth_dl": {
                "type": "number",
                "title": "Max Requested Bandwidth DL (Kbps)",
                "default": 1024000,
                "required": true
              }
            }
          }
        }
      }
    }
  }
};

const uiSchema = {
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
          }
        }
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
    disableSubmitButton: PropTypes.bool,
    validate: PropTypes.func, 
    onHide: PropTypes.func, 
    onChange: PropTypes.func,
    onSubmit: PropTypes.func
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
      onSubmit
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
        onSubmit={onSubmit} />
    )
  }
}

export default withWidth()(Edit);
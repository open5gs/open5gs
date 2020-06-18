import { Component } from 'react';
import PropTypes from 'prop-types';

import withWidth, { SMALL } from 'helpers/with-width';
import { Form } from 'components';

import traverse from 'traverse';

const schema = {
  "title": "Subscriber Configuration",
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
        "amf": {
          "type": "string",
          "title": "Authentication Management Field (AMF)*",
          "required": true,
          "pattern": "^[0-9a-fA-F\\s]+$",
          "messages": {
            "pattern": "Only hexadecimal digits are allowed"
          }
        },
        "op_type": {
          "type": "number",
          "title": "USIM Type",
          "enum": [0, 1],
          "enumNames": ["OPc", "OP"],
          "default": 0,
        },
        "op_value": {
          "type": "string",
          "title": "Operator Key (OPc/OP)*",
          "required": true,
          "pattern": "^[0-9a-fA-F\\s]+$",
          "messages": {
            "pattern": "Only hexadecimal digits are allowed"
          }
        },
      }
    },
    "ambr": {
      "type": "object",
      "title": "",
      "properties": {
        "downlink": {
          "type": "number",
          "title": "UE-AMBR Downlink (Kbps)*",
          "required": true
        },
        "uplink": {
          "type": "number",
          "title": "UE-AMBR Uplink (Kbps)*",
          "required": true
        }
      }
    },
    "pdn": {
      "type": "array",
      "title": "APN Configurations",
      "minItems": 1,
      "maxItems": 4,
      "messages": {
        "minItems": "At least 1 APN is required",
        "maxItems": "4 APNs are supported"
      },
      "items": {
        "type": "object",
        "properties": {
          "apn": {
            "type": "string",
            "title": "Access Point Name (APN)*",
            "required": true
          },
          "type": {
            "type": "number",
            "title": "Type*",
            "enum": [0, 1, 2],
            "enumNames": ["IPv4", "IPv6", "IPv4v6"],
            "default": 2,
          },
          "qos": {
            "type": "object",
            "title": "",
            "properties": {
              "qci": {
                "type": "number",
                "title": "QoS Class Identifier (QCI)*",
                "enum": [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 65, 66, 69, 70 ],
                "default": 5,
              },
              "arp" : {
                "type": "object",
                "title": "",
                "properties": {
                  "priority_level": {
                    "type": "number",
                    "title": "ARP Priority Level (1-15)*",
                    "default": 1,
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
                  },
                  "pre_emption_vulnerability": {
                    "type": "number",
                    "title": "Vulnerability*",
                    "enum": [1, 0],
                    "enumNames": ["Disabled", "Enabled"],
                    "default": 1,
                  },
                }
              }
            }
          },
          "ambr": {
            "type": "object",
            "title": "",
            "properties": {
              "downlink": {
                "type": "number",
                "title": "APN-AMBR Downlink (Kbps)*",
                "required": true
              },
              "uplink": {
                "type": "number",
                "title": "APN-AMBR Uplink (Kbps)*",
                "required": true
              },
            }
          },
          "ue": {
            "type": "object",
            "title": "",
            "properties": {
              "addr": {
                "type": "string",
                "title": "UE IPv4 Address",
                "format" : "ipv4"
              },
              "addr6": {
                "type": "string",
                "title": "UE IPv6 Address",
                "format" : "ipv6"
              },
            }
          },
          "pgw": {
            "type": "object",
            "title": "",
            "properties": {
              "addr": {
                "type": "string",
                "title": "PGW IPv4 Address",
                "format" : "ipv4"
              },
              "addr6": {
                "type": "string",
                "title": "PGW IPv6 Address",
                "format" : "ipv6"
              },
            }
          },
          "pcc_rule": {
            "type": "array",
            "title": "PCC Rules",
            "maxItems": 8,
            "messages": {
              "maxItems": "8 PCC Rules are supported"
            },
            "items": {
              "type": "object",
              "properties": {
                "flow": {
                  "type": "array",
                  "title": "",
                  "maxItems": 8,
                  "messages": {
                    "maxItems": "8 Flows are supported"
                  },
                  "items": {
                    "type": "object",
                    "properties": {
                      "direction": {
                        "type": "number",
                        "title": "Flow Direction*",
                        "enum": [1, 2],
                        "enumNames": ["Downlink", "Uplink"],
                        "default": 1,
                      },
                      "description": {
                        "type": "string",
                        "title": "Description*",
                        "default": "permit out udp from any 1-65535 to 45.45.45.45",
                        "required": true,
                        "pattern": "^permit\\s+out",
                        "messages": {
                          "pattern": "Begin with reserved keyword 'permit out'."
                        }
                      }
                    }
                  }
                },
                "qos": {
                  "type": "object",
                  "title": "",
                  "properties": {
                    "qci": {
                      "type": "number",
                      "title": "QoS Class Identifier (QCI)*",
                      "enum": [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 65, 66, 69, 70 ],
                      "default": 1,
                    },
                    "arp" : {
                      "type": "object",
                      "title": "",
                      "properties": {
                        "priority_level": {
                          "type": "number",
                          "title": "ARP Priority Level (1-15)*",
                          "default": 2,
                          "minimum": 1,
                          "maximum": 15,
                          "required": true
                        },
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
      // enable Pre-emption Capability/Vulnerablility in Default Bearer
                        "pre_emption_capability": {
                          "type": "number",
                          "title": "Capability*",
                          "enum": [1, 0],
                          "enumNames": ["Disabled", "Enabled"],
                          "default": 0,
                        },
                        "pre_emption_vulnerability": {
                          "type": "number",
                          "title": "Vulnerability*",
                          "enum": [1, 0],
                          "enumNames": ["Disabled", "Enabled"],
                          "default": 0,
                        },
                      }
                    },
                    "mbr": {
                      "type": "object",
                      "title": "",
                      "properties": {
                        "downlink": {
                          "type": "number",
                          "title": "MBR Downlink (Kbps)",
                        },
                        "uplink": {
                          "type": "number",
                          "title": "MBR Uplink (Kbps)",
                        },
                      }
                    },
                    "gbr": {
                      "type": "object",
                      "title": "",
                      "properties": {
                        "downlink": {
                          "type": "number",
                          "title": "GBR Downlink (Kbps)",
                        },
                        "uplink": {
                          "type": "number",
                          "title": "GBR Uplink (Kbps)",
                        },
                      }
                    },
                  },
                },
              }
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
      classNames: "col-xs-7",
    },
    "amf" : {
      classNames: "col-xs-5",
    },
    "op_type" : {
      classNames: "col-xs-4",
    },
    "op_value" : {
      classNames: "col-xs-8",
    },
  },
  "ambr" : {
    "downlink" : {
      classNames: "col-xs-6"
    },
    "uplink" : {
      classNames: "col-xs-6"
    },
  },
  "pdn": {
    "items": {
      "apn": {
        classNames: "col-xs-8",
      },
      "type": {
        classNames: "col-xs-4",
      },
      "qos": {
        classNames: "col-xs-12",
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
      "ambr" : {
        classNames: "col-xs-12",
        "downlink" : {
          classNames: "col-xs-6"
        },
        "uplink" : {
          classNames: "col-xs-6"
        },
      },
      "ue" : {
        classNames: "col-xs-12",
        "addr" : {
          classNames: "col-xs-6"
        },
        "addr6" : {
          classNames: "col-xs-6"
        },
      },
      "pgw" : {
        classNames: "col-xs-12",
        "addr" : {
          classNames: "col-xs-6"
        },
        "addr6" : {
          classNames: "col-xs-6"
        },
      },
      "pcc_rule": {
        classNames: "col-xs-12",
        "items": {
          "flow": {
            "items": {
              "direction": {
                classNames: "col-xs-12"
              },
              "description": {
                classNames: "col-xs-12",
                "ui:help": "Hint: Flow-Description(TS29.212), IPFilterRule(RFC 3588)",
              },
            },
          },
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
            },
            "mbr": {
              "downlink": {
                classNames: "col-xs-6"
              },
              "uplink": {
                classNames: "col-xs-6"
              }
            },
            "gbr": {
              "downlink": {
                classNames: "col-xs-6"
              },
              "uplink": {
                classNames: "col-xs-6"
              }
            }
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
    validate: PropTypes.func, 
    onHide: PropTypes.func, 
    onSubmit: PropTypes.func,
    onError: PropTypes.func
  }

  constructor(props) {
    super(props);

    this.state = this.getStateFromProps(props);
  }

  componentWillReceiveProps(nextProps) {
    this.setState(this.getStateFromProps(nextProps));
  }

  getStateFromProps(props) {
    const { 
      action,
      profiles,
      width,
      formData,
    } = props;

    let state = {
      schema,
      uiSchema,
      formData,
    };

    if (action === 'create' && Object.keys(profiles).length > 0) {
      if (this.state.profile === undefined) {
        state = Object.assign(state, {
          profile : profiles[0]._id
        })
      } else {
        state = Object.assign(state, {
          profile : this.state.profile
        })
      }

      state = {
        ...state,
        "schema" : {
          ...schema,
          "properties": {
            profile: {
              type: "string", 
              title: "Profile*",
              enum: profiles.map(profile => profile._id),
              enumNames: profiles.map(profile => profile.title),
              default: state.profile
            },
            ...schema.properties
          }
        }
      }

      state = Object.assign(state, {
        formData : this.getFormDataFromProfile(state.profile)
      })

      delete state.uiSchema.profile;
    } else {
      delete state.schema.properties.profile;
    }

    if (action === 'update') {
      state.uiSchema = Object.assign(state.uiSchema, {
        "imsi": {
          "ui:disabled": true
        }
      });
    } else if (width !== SMALL) {
      state.uiSchema = Object.assign(state.uiSchema, {
        "imsi": {
          "ui:autofocus": true
        }
      });
    }

    return state;
  }

  getFormDataFromProfile(profile) {
    let formData;
    
    formData = Object.assign({}, this.props.profiles.filter(p => p._id === profile)[0]);
    formData = Object.assign(formData, { profile });

    delete formData.title;
    delete formData._id;
    delete formData.__v;

    traverse(formData).forEach(function(x) {
      if (this.key == 'downlink') this.update(Number(x));
      if (this.key == 'uplink') this.update(Number(x));
    })
    if (formData.security) {
      if (formData.security.opc) {
        formData.security.op_type = 0;
        formData.security.op_value = formData.security.opc;
      } else {
        formData.security.op_type = 1;
        formData.security.op_value = formData.security.op;
      }
    }

    return formData;
  }

  handleChange = (formData) => {
    const {
      action,
      profiles
    } = this.props;

    if (action === 'create' && Object.keys(profiles).length > 0) {
      if (this.state.profile !== formData.profile) {
        let data = this.getFormDataFromProfile(formData.profile)
        this.setState({
          profile: formData.profile,
          formData : data
        });

        return data;
      }
    }

    return undefined;
  }

  render() {
    const {
      handleChange
    } = this;

    const {
      visible,
      action,
      isLoading,
      validate,
      onHide,
      onSubmit,
      onError
    } = this.props;

    const {
      formData
    } = this.state;

    return (
      <Form 
        visible={visible}
        title={(action === 'update') ? 'Edit Subscriber' : 'Create Subscriber'}
        schema={this.state.schema}
        uiSchema={this.state.uiSchema}
        formData={formData}
        isLoading={isLoading}
        validate={validate}
        onHide={onHide}
        onChange={handleChange}
        onSubmit={onSubmit}
        onError={onError}/>
    )
  }
}

export default withWidth()(Edit);

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
    "msisdn": {
      "type": "array",
      "title": "",
      "maxItems": 2,
      "messages": {
        "maxItems": "2 MSISDN are supported"
      },
      "items": {
        "type": "string",
        "title": "MSISDN",
        "maxLength": 15,
        "required": true,
        "pattern": "^\\d+$",
        "messages": {
          "pattern": "Only digits are allowed"
        }
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
          "type": "object",
          "title": "",
          "properties": {
            "value": {
              "type": "number",
              "title": "UE-AMBR Downlink*",
              "required": true,
            },
            "unit": {
              "type": "number",
              "title": "Unit",
              "enum": [0, 1, 2, 3, 4],
              "enumNames": ["bps", "Kbps", "Mbps", "Gbps", "Tbps"],
              "default": 3,
            }
          }
        },
        "uplink": {
          "type": "object",
          "title": "",
          "properties": {
            "value": {
              "type": "number",
              "title": "UE-AMBR Uplink*",
              "required": true,
            },
            "unit": {
              "type": "number",
              "title": "Unit",
              "enum": [0, 1, 2, 3, 4],
              "enumNames": ["bps", "Kbps", "Mbps", "Gbps", "Tbps"],
              "default": 3,
            }
          }
        }
      }
    },
    "slice": {
      "type": "array",
      "title": "Slice Configurations",
      "minItems": 1,
      "maxItems": 8,
      "messages": {
        "minItems": "At least 1 Slice is required",
        "maxItems": "8 Slices are supported"
      },
      "items": {
        "type": "object",
        "properties": {
          "sst": {
            "type": "number",
            "title": "SST*",
            "enum": [ 1, 2, 3, 4 ],
            "required": true
          },
          "sd": {
            "type": "string",
            "title": "SD",
            "pattern": "^[0-9a-fA-F]+$",
            "minLength": 6,
            "maxLength": 6,
            "messages": {
              "pattern": "Only hexadecimal digits are allowed"
            }
          },
          "default_indicator": {
            "type": "boolean",
            "title": "Default S-NSSAI",
          },
          "session": {
            "type": "array",
            "title": "Session Configurations",
            "minItems": 1,
            "maxItems": 4,
            "messages": {
              "minItems": "At least 1 Session is required",
              "maxItems": "4 Sessions are supported"
            },
            "items": {
              "type": "object",
              "properties": {
                "name": {
                  "type": "string",
                  "title": "DNN/APN*",
                  "required": true
                },
                "type": {
                  "type": "number",
                  "title": "Type*",
                  "enum": [1, 2, 3],
                  "enumNames": ["IPv4", "IPv6", "IPv4v6"],
                  "default": 3,
                },
                "qos": {
                  "type": "object",
                  "title": "",
                  "properties": {
                    "index": {
                      "type": "number",
                      "title": "5QI/QCI*",
                      "enum": [ 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86 ],
                      "default": 5,
                    },
                    "arp" : {
                      "type": "object",
                      "title": "",
                      "properties": {
                        "priority_level": {
                          "type": "number",
                          "title": "ARP Priority Level (1-15)*",
                          "enum": [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
                          "default": 1,
                        },
                        "pre_emption_capability": {
                          "type": "number",
                          "title": "Capability*",
                          "enum": [1, 2],
                          "enumNames": ["Disabled", "Enabled"],
                          "default": 1
                        },
                        "pre_emption_vulnerability": {
                          "type": "number",
                          "title": "Vulnerability*",
                          "enum": [1, 2],
                          "enumNames": ["Disabled", "Enabled"],
                          "default": 1
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
                      "type": "object",
                      "title": "",
                      "properties": {
                        "value": {
                          "type": "number",
                          "title": "Session-AMBR Downlink*",
                          "default": 1,
                          "required": true,
                        },
                        "unit": {
                          "type": "number",
                          "title": "Unit",
                          "enum": [0, 1, 2, 3, 4],
                          "enumNames": ["bps", "Kbps", "Mbps", "Gbps", "Tbps"],
                          "default": 3,
                        }
                      }
                    },
                    "uplink": {
                      "type": "object",
                      "title": "",
                      "properties": {
                        "value": {
                          "type": "number",
                          "title": "Session-AMBR Uplink*",
                          "default": 1,
                          "required": true,
                        },
                        "unit": {
                          "type": "number",
                          "title": "Unit",
                          "enum": [0, 1, 2, 3, 4],
                          "enumNames": ["bps", "Kbps", "Mbps", "Gbps", "Tbps"],
                          "default": 3,
                        }
                      }
                    }
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
                "smf": {
                  "type": "object",
                  "title": "",
                  "properties": {
                    "addr": {
                      "type": "string",
                      "title": "SMF IPv4 Address",
                      "format" : "ipv4"
                    },
                    "addr6": {
                      "type": "string",
                      "title": "SMF IPv6 Address",
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
                          "index": {
                            "type": "number",
                            "title": "5QI/QCI*",
                            "enum": [ 1, 2, 3, 4, 65, 66, 67, 75, 71, 72, 73, 74, 76, 5, 6, 7, 8, 9, 69, 70, 79, 80, 82, 83, 84, 85, 86 ],
                            "default": 1,
                          },
                          "arp" : {
                            "type": "object",
                            "title": "",
                            "properties": {
                              "priority_level": {
                                "type": "number",
                                "title": "ARP Priority Level (1-15)*",
                                "enum": [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
                                "default": 2,
                              },
                              "pre_emption_capability": {
                                "type": "number",
                                "title": "Capability*",
                                "enum": [1, 2],
                                "enumNames": ["Disabled", "Enabled"],
                                "default": 2,
                              },
                              "pre_emption_vulnerability": {
                                "type": "number",
                                "title": "Vulnerability*",
                                "enum": [1, 2],
                                "enumNames": ["Disabled", "Enabled"],
                                "default": 2,
                              },
                            }
                          },
                          "mbr": {
                            "type": "object",
                            "title": "",
                            "properties": {
                              "downlink": {
                                "type": "object",
                                "title": "",
                                "properties": {
                                  "value": {
                                    "type": "number",
                                    "title": "MBR Downlink",
                                  },
                                  "unit": {
                                    "type": "number",
                                    "title": "Unit",
                                    "enum": [0, 1, 2, 3, 4],
                                    "enumNames": ["bps", "Kbps", "Mbps", "Gbps", "Tbps"],
                                    "default": 1,
                                  }
                                }
                              },
                              "uplink": {
                                "type": "object",
                                "title": "",
                                "properties": {
                                  "value": {
                                    "type": "number",
                                    "title": "MBR Uplink",
                                  },
                                  "unit": {
                                    "type": "number",
                                    "title": "Unit",
                                    "enum": [0, 1, 2, 3, 4],
                                    "enumNames": ["bps", "Kbps", "Mbps", "Gbps", "Tbps"],
                                    "default": 1,
                                  }
                                }
                              }
                            }
                          },
                          "gbr": {
                            "type": "object",
                            "title": "",
                            "properties": {
                              "downlink": {
                                "type": "object",
                                "title": "",
                                "properties": {
                                  "value": {
                                    "type": "number",
                                    "title": "GBR Downlink",
                                  },
                                  "unit": {
                                    "type": "number",
                                    "title": "Unit",
                                    "enum": [0, 1, 2, 3, 4],
                                    "enumNames": ["bps", "Kbps", "Mbps", "Gbps", "Tbps"],
                                    "default": 1,
                                  }
                                }
                              },
                              "uplink": {
                                "type": "object",
                                "title": "",
                                "properties": {
                                  "value": {
                                    "type": "number",
                                    "title": "GBR Uplink",
                                  },
                                  "unit": {
                                    "type": "number",
                                    "title": "Unit",
                                    "enum": [0, 1, 2, 3, 4],
                                    "enumNames": ["bps", "Kbps", "Mbps", "Gbps", "Tbps"],
                                    "default": 1,
                                  }
                                }
                              }
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
      }
    }
  }
};

const uiSchema = {
  "imsi" : {
    classNames: "col-xs-12",
  },
  "msisdn" : {
    classNames: "col-xs-7",
  },
  "security" : {
    classNames: "col-xs-12",
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
    classNames: "col-xs-12",
    "downlink": {
      classNames: "col-xs-6",
      "value": {
        classNames: "col-xs-8",
      },
      "unit": {
        classNames: "col-xs-4",
      },
    },
    "uplink": {
      classNames: "col-xs-6",
      "value": {
        classNames: "col-xs-8",
      },
      "unit": {
        classNames: "col-xs-4",
      },
    }
  },
  "slice": {
    classNames: "col-xs-12",
    "items": {
      "sst": {
        classNames: "col-xs-3",
        "ui:widget": "radio",
        "ui:options": { "inline": true },
      },
      "sd": {
        classNames: "col-xs-6",
      },
      "default_indicator": {
        classNames: "col-xs-3",
      },
      "session": {
        classNames: "col-xs-12",
        "items": {
          "name": {
            classNames: "col-xs-8",
          },
          "type": {
            classNames: "col-xs-4",
          },
          "qos": {
            classNames: "col-xs-12",
            "index": {
            },
            "arp": {
              "priority_level": {
              },
              "pre_emption_capability": {
                classNames: "col-xs-6"
              },
              "pre_emption_vulnerability": {
                classNames: "col-xs-6"
              }
            }
          },
          "ambr" : {
            classNames: "col-xs-12",
            "downlink": {
              "value": {
                classNames: "col-xs-8"
              },
              "unit": {
                classNames: "col-xs-4"
              },
            },
            "uplink": {
              "value": {
                classNames: "col-xs-8"
              },
              "unit": {
                classNames: "col-xs-4"
              },
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
          "smf" : {
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
                  },
                  "description": {
                    "ui:help": "Hint: 5.4.2 Flow-Description in TS29.212",
                  },
                },
              },
              "qos": {
                "index": {
                },
                "arp": {
                  "priority_level": {
                    classNames: "col-xs-12"
                  },
                  "pre_emption_capability": {
                    classNames: "col-xs-6"
                  },
                  "pre_emption_vulnerability": {
                    classNames: "col-xs-6"
                  }
                },
                "mbr": {
                  "downlink": {
                    "value": {
                      classNames: "col-xs-8"
                    },
                    "unit": {
                      classNames: "col-xs-4"
                    },
                  },
                  "uplink": {
                    "value": {
                      classNames: "col-xs-8"
                    },
                    "unit": {
                      classNames: "col-xs-4"
                    },
                  }
                },
                "gbr": {
                  "downlink": {
                    "value": {
                      classNames: "col-xs-8"
                    },
                    "unit": {
                      classNames: "col-xs-4"
                    },
                  },
                  "uplink": {
                    "value": {
                      classNames: "col-xs-8"
                    },
                    "unit": {
                      classNames: "col-xs-4"
                    },
                  }
                }
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

    //traverse(formData).forEach(function(x) {
    //  if (this.key == 'downlink') this.update(Number(x));
    //  if (this.key == 'uplink') this.update(Number(x));
    //})
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

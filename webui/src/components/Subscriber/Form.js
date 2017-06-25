import React, { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import SchemaForm from 'react-jsonschema-form';

import { Modal } from 'components';

const Wrapper = styled.div`
  width: 800px;
  height: 500px;
  overflow: scroll;

  padding: 2rem;
  font-size: 14px;

  background: white;
  box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23);
`

const schema = {
  "title": "Subscriber",
  "type": "object",
  "required": ["imsi"],
  "properties": {
    "imsi": {
      "type": "string", 
      "title": "IMSI (International Mobile Subscriber Identity)" 
    },
    "security": {
      "title": "Security",
      "type": "object",
      "required": ["k"],
      "properties": {
        "k": {
          "type": "string",
          "title": "K (UE Key)"
        },
        "op": {
          "type": "string",
          "title": "OP (Operator Key)"
        },
        "amf": {
          "type": "string",
          "title": "AMF (Authentication Management Field)"
        }
      }
    },
    "ue_ambr": {
      "type": "object",
      "title": "UE AMBR - Aggregate Maximum Bit Rate",
      "properties": {
        "max_bandwidth_ul": {
          "type": "number",
          "title": "Max Requested Bandwidth UL (Kbps)"
        },
        "max_bandwidth_dl": {
          "type": "number",
          "title": "Max Requested Bandwidth DL (Kbps)"
        }
      }
    },
    "pdn": {
      "type": "array",
      "title": "PDN - Packet Data Network",
      "items": {
        "type": "object",
        "required": ["apn"],
        "properties": {
          "apn": {
            "type": "string",
            "title": "APN (Access Point Name)"
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
                    "minimum": 1,
                    "maximum": 15,
                    "default": 8
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
                "default": 1024000
              },
              "max_bandwidth_dl": {
                "type": "number",
                "title": "Max Requested Bandwidth DL (Kbps)",
                "default": 1024000
              }
            }
          }
        }
      }
    }
  }
};

const uiSchema = {
  "imsi": {
    "ui:autofocus": true,
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
          }
        }
      }
    }
  }
}

const formData = {
  "ue_ambr": {
    "max_bandwidth_ul": 1024000,
    "max_bandwidth_dl": 1024000
  },
  "security": {
    "k": "465B5CE8B199B49FAA5F0A2EE238A6BC",
    "op": "5F1D289C5D354D0A140C2548F5F3E3BA",
    "amf": "8000"
  },
  "pdn": [
    {
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

const propTypes = {
  visible: PropTypes.bool, 
  onHide: PropTypes.func, 
  onSubmit: PropTypes.func,
};

const log = (type) => console.log.bind(console, type);

const Form = ({ visible, onHide, onSubmit }) => (
  <Modal 
    visible={visible} 
    onHide={onHide}>
    <Wrapper>
      <SchemaForm 
        schema={schema}
        uiSchema={uiSchema}
        formData={formData}
        onChange={log("changed")}
        onSubmit={log("submitted")}
        onError={log("errors")}>
      </SchemaForm>
    </Wrapper>  
  </Modal>
)

Form.PropTypes = propTypes;

export default Form;
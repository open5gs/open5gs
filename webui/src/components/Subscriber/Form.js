import React, { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import SchemaForm from 'react-jsonschema-form';

import { Modal } from 'components';

const Wrapper = styled.div`
  width: 800px;
  height: 600px;
  overflow: scroll;

  padding: 2rem;
  font-size: 14px;

  background: white;
  box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23);
`

const schema = {
  title: "Subscriber",
  type: "object",
  required: ["imsi"],
  properties: {
    imsi: {
      type: "string", 
      title: "IMSI" 
    },
    ambr: {
      title: "AMBR",
      type: "object",
      properties: {
        ul: {
          type: "string",
          title: "Uplink"
        },
        dl: {
          type: "string",
          title: "Downlink"
        }
      }
    },
    security: {
      title: "Security",
      type: "object",
      properties: {
        k: {
          type: "string",
          title: "K"
        },
        sqn: {
          type: "string",
          title: "SQN"
        },
        rand: {
          type: "string",
          title: "RAND"
        },
        op: {
          type: "string",
          title: "OP"
        },
        amf: {
          type: "string",
          title: "AMF"
        }
      }
    },
    pdns: {
      type: "array",
      title: "PDN",
      items: {
        type: "object",
        required: ["apn"],
        properties: {
          apn: {
            type: "string",
            title: "APN"
          },
          type: {
            type: "string",
            title: "Type"
          },
          ambr: {
            title: "AMBR",
            type: "object",
            properties: {
              ul: {
                type: "string",
                title: "Uplink"
              },
              dl: {
                type: "string",
                title: "Downlink"
              }
            }
          },
          qci: {
            type: "string",
            title: "QCI"
          },
          priority_level: {
            type: "string",
            title: "Priority Level"
          },
        }
      }
    }
  }
};

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
    <SchemaForm schema={schema}
        onChange={log("changed")}
        onSubmit={log("submitted")}
        onError={log("errors")} />
    </Wrapper>  
  </Modal>
)

Form.PropTypes = propTypes;

export default Form;
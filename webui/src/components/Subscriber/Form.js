import React, { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import SchemaForm from 'react-jsonschema-form';

import { Modal } from 'components';

const schema = {
  title: "Todo",
  type: "object",
  required: ["title"],
  properties: {
    title: {type: "string", title: "Title", default: "A new task"},
    done: {type: "boolean", title: "Done?", default: false}
  }
};

const log = (type) => console.log.bind(console, type);

const Wrapper = styled.div`
  background: white;
  padding: 2rem;
  width: 600px;
`

const propTypes = {
  visible: PropTypes.bool, 
  onHide: PropTypes.func, 
  onSubmit: PropTypes.func,
};

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
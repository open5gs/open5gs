import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import { media, transitions } from 'helpers/style-utils';

import Modal from './Modal';
import Button from './Button';

const Wrapper = styled.div`
  display: flex;
  flex-direction: column;
  postion: relative;
  width: 300px;

  ${media.mobile`
    width: calc(100vw - 2rem);
  `}

  background: white;
  box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23);
`

const Message = styled.div`
  padding: 2rem;
`

const Buttons = styled.div`
  display: flex;
  justify-content: flex-end;

  padding: 1rem;
`

const Confirm = ({ visible, message, buttons, onHide }) => {
  const buttonList = buttons
    .map(button =>
      <Button
        clear 
        danger={button.danger === true} 
        info={button.info === true} 
        onClick={button.action}>
        {button.text}
      </Button>
    );
  return (
    <Modal visible={visible} onHide={onHide}>
      <Wrapper>
        <Message>{message}</Message>
        <Buttons>{buttonList}</Buttons>
      </Wrapper>  
    </Modal>
  )
}

export default Confirm;
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import { media, transitions } from 'helpers/style-utils';

import Modal from './Modal';
import Button from './Button';
import Dimmed from './Dimmed';

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

const Confirm = ({ visible, onOutside, message, buttons }) => {
  const buttonList = buttons
    .map(button =>
      <Button
        key={button.text}
        clear={true}
        danger={button.danger === true} 
        info={button.info === true} 
        onClick={button.action}>
        {button.text}
      </Button>
    );
  return (
    <div>
      <Modal
        visible={visible}
        onOutside={onOutside}
        zindex='1000'
        transitionEnterTimeout={10}
        transitionLeaveTimeout={30}>
        <Wrapper>
          <Message>{message}</Message>
          <Buttons>{buttonList}</Buttons>
        </Wrapper>
      </Modal>
      <Dimmed visible={visible} zindex='999'/>
    </div>
  )
}

Confirm.propTypes = {
  visible: PropTypes.bool,
  disabled: PropTypes.bool,
  onOutside: PropTypes.func,
}

Confirm.defaultProps = {
  visible: false,
  disabled: true,
  onOutside: () => {},
}

export default Confirm;
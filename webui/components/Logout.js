import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import Modal from './Modal';

const TitleWrapper = styled.div`
  display: flex;
  justify-content: center;
  padding-top: 2rem;
  padding-bottom: 2rem;

  font-size: 1.2rem;
  color: ${oc.gray[8]};

  background: ${oc.gray[2]};
`;

const ButtonsWrapper = styled.div`
  display: flex;
`;

const Button = styled.div`
  padding-top: 0.2rem;
  padding-bottom: 0.2rem;
  flex: 1;
  display: inline-block;
  
  cursor: pointer;
  text-align: center;
  font-weight: 500;
  font-size: 1.5rem;
  transition: all .3s;

  color: white;
  background: ${props => oc[props.color][7]};

  &:hover {
    background: ${props => oc[props.color][6]};
  }

  &:active {
    background: ${props => oc[props.color][8]};
  }
`;

Button.propType = {
  color: PropTypes.string
};

class Logout extends Component {

  static propTypes = { 
    visible: PropTypes.bool, 
    color: PropTypes.string, 
    onHide: PropTypes.func, 
    onAction: PropTypes.func,
  }

  render() {
    const { 
      visible,
      color,
      onHide,
      onAction,
    } = this.props;

    return (
      <Modal visible={visible} onHide={onHide}>
        <TitleWrapper>
          Are you sure you want to logout?
        </TitleWrapper>
        <ButtonsWrapper>
          <Button color="teal"
            onClick={onAction}>
            OK
          </Button>
          <Button 
            onClick={onHide}
            color="gray">
            Cancel
          </Button>
        </ButtonsWrapper>
      </Modal>
    );
  }
}

export default Logout;

import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import Thumbnail from './Thumbnail';
import Modal from './Modal';

const ThumbnailWrapper = styled.div`
  padding-top: 3rem;
  padding-bottom: 3rem;
  display: flex;
  justify-content: center;

  background: white;
`;

const TitleWrapper = styled.div`
  color: ${oc.gray[9]};
  background: ${oc.gray[0]};

  font-size: 1.2rem;
  line-height: 4rem;
  text-align: center;
  color: ${oc.gray[7]};

  background-color: ${oc.pink[2]};
  border-bottom: 1px solid ${oc.pink[3]};
  box-shadow: 1px 1px 2px ${oc.pink[3]};
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
        <ThumbnailWrapper>
          <Thumbnail size='8rem' color={oc['blue'][6]} />
        </ThumbnailWrapper>
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

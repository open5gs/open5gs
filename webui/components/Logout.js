import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import Modal from './Modal';

const Wrapper = styled.div`
`;

const TitleWrapper = styled.div`
  padding-left: 1rem;
  font-size: 1.5rem;
  line-height: 3.5rem;
  font-weight: 600;
  color: white;

  background-color: ${oc.red[7]};
`;

const ContentWrapper = styled.div`
  padding: 0 0 0.8rem 1rem;

  font-size: 1rem;
  color: ${oc.gray[7]};

  background-color: ${oc.gray[1]};
`;

const ButtonsWrapper = styled.div`
  display: flex;
  justify-content: flex-end;
  background-color: ${oc.gray[2]};
`;

const Button = styled.button`
  display: inline;
  font-size: 1rem;
  margin: 1rem 0.5rem;
  padding: 0.3rem;
  width: 5rem;
  border-radius: 3px;

  transition: all .3s;
`;

const YesButton = Button.extend`
  border: 1px solid ${oc.red[9]};
  color: white;
  background: ${oc.red[7]};
  &:hover {
    background: ${oc.red[5]}
  }
  &:active {
    background: ${oc.red[8]}
  }
`;

const NoButton = Button.extend`
  border: 1px solid ${oc.gray[5]};
  color: black;
  background: ${oc.gray[3]};
  &:hover {
    background: ${oc.gray[2]}
  }
  &:active {
    background: ${oc.gray[4]}
  }
`;


class Logout extends Component {

  static propTypes = { 
    visible: PropTypes.bool, 
    onHide: PropTypes.func, 
    onAction: PropTypes.func,
  }

  render() {
    const { 
      visible,
      onHide,
      onAction,
    } = this.props;

    return (
      <Modal visible={visible} onHide={onHide}>
        <Wrapper>
          <TitleWrapper>
            Logout
          </TitleWrapper>
          <ContentWrapper>
            Are you sure you want to logout?
          </ContentWrapper>
          <ButtonsWrapper>
            <YesButton
              onClick={onAction}>
              Yes
            </YesButton>
            <NoButton 
              onClick={onHide}>
              No
            </NoButton>
          </ButtonsWrapper>
        </Wrapper>
      </Modal>
    );
  }
}

export default Logout;

import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import { Modal } from 'components';
import { media, transitions } from 'helpers/style-utils';

import PersonIcon from 'react-icons/lib/md/person';

const Wrapper = styled.div`
  width: 300px;

  ${media.mobile`
    width: calc(100vw - 2rem);
  `}
`;

const TitleWrapper = styled.div`
  padding-left: 1rem;
  line-height: 3rem;

  font-size: 1.2rem;

  color: white;
  background-color: ${oc.red[7]};
`;

const ContentWrapper = styled.div`
  padding: 1rem 0 0 1rem;
  height: 5rem;

  font-size: 1rem;
  color: ${oc.gray[7]};

  background-color: ${oc.gray[1]};
`;

const ButtonWrapper = styled.div`
  display: flex;
  justify-content: flex-end;
  background-color: ${oc.gray[2]};
`;

const Button = styled.button`
  margin: 0.5rem;
  padding: 0.3rem;
  width: 4rem;

  text-align: center;
  font-size: 0.9rem;

  border-radius: 3px;
  outline: none;
  cursor: pointer;

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

const propTypes = {
  visible: PropTypes.bool, 
  onHide: PropTypes.func, 
  onLogout: PropTypes.func,
};

const Logout = ({ visible, onHide, onLogout }) => (
  <Modal 
    visible={visible} 
    onOutside={onHide} 
    transitionEnter={`${transitions.slideDown} .5s ease-in-out`}
    transitionLeave={`${transitions.slideUp} .5s ease-in-out`}
    transitionEnterTimeout={500}
    transitionLeaveTimeout={500}>
    <Wrapper>
      <TitleWrapper>
        Logout
      </TitleWrapper>
      <ContentWrapper>
        Are you sure you want to logout?
      </ContentWrapper>
      <ButtonWrapper>
        <YesButton
          onClick={onLogout}>
          Yes
        </YesButton>
        <NoButton 
          onClick={onHide}>
          No
        </NoButton>
      </ButtonWrapper>
    </Wrapper>
  </Modal>
)

Logout.propTypes = propTypes;

export default Logout;

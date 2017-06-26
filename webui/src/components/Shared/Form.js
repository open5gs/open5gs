import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import Modal from './Modal';
import Button from './Button';

const Wrapper = styled.div`
  display: flex;
  flex-direction: column;
  postion: relative;
  width: 800px;

  ${media.mobile`
    width: calc(100vw - 2rem);
  `}

  background: white;
  box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23);
`

const Header = styled.div`
  display: flex;
  justify-content: flex-start;

  padding: 1rem;
  font-size: 1.5rem;
  background: ${oc.gray[1]};
`

const Body = styled.div`
  padding: 2rem;
  font-size: 14px;

  height: 400px;
  ${media.mobile`
    height: calc(100vh - 8rem);
  `}

  overflow: scroll;
`

const Footer = styled.div`
  display: flex;
  justify-content: flex-end;

  padding: 1rem;
`

const Form = ({ visible, title, children, onHide, onSubmit }) => (
  <Modal 
    visible={visible} 
    onHide={onHide}>
    <Wrapper>
      <Header>
        {title}
      </Header>
      <Body>
        {children}
      </Body>
      <Footer>
        <Button clear onClick={onHide}>
          CANCEL
        </Button>
        <Button clear onClick={onSubmit}>
          SAVE
        </Button>
      </Footer>
    </Wrapper>  
  </Modal>
)

Form.propTypes = {
  visible: PropTypes.bool,
  title: PropTypes.string,
  onHide: PropTypes.func,
  onSubmit: PropTypes.func
}

Form.defaultProps = {
  visible: false,
  title: ""
}

export default Form;
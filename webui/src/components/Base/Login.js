import { Component } from 'react';
import Head from 'next/head';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import { CircleIcon } from 'components';
import PersonIcon from 'react-icons/lib/md/person';
import CloseIcon from 'react-icons/lib/md/close';

const Wrapper = styled.div`
  position: fixed;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);

  border: 1px solid ${oc.gray[4]};
  box-shadow: 1px 1px 2px ${oc.gray[4]};

  width: ${props => props.width};
  ${media.mobile`
    top: 0;
    left: 0;
    transform: translate(0, 0);

    width: 100%;
  `}
`;

Wrapper.propTypes = {
  width: PropTypes.string
}

const ErrorWrapper = styled.div`
  display: flex;
  align-items: center;

  font-size: 1rem;
  line-height: 3rem;
  color: ${oc.gray[7]};

  background-color: ${oc.pink[2]};
  border-bottom: 1px solid ${oc.pink[3]};
  box-shadow: 1px 1px 2px ${oc.pink[3]};
`;

const ErrorMessage = styled.div`
  padding-left: 1rem;
`;

const ErrorClose = styled.div`
  position: absolute;
  right: 1rem;
`;

const ErrorBar = ({ visible, message, onClose }) => visible ? (
  <ErrorWrapper>
    <ErrorMessage>
      {message}
    </ErrorMessage>
    <ErrorClose onClick={onClose}>
      <CloseIcon/>
    </ErrorClose>
  </ErrorWrapper>
) : null;

ErrorBar.propTypes = {
  visible: PropTypes.bool,
  message: PropTypes.string,
  onClose: PropTypes.func
};

const Thumbnail = styled.div`
  display: flex;
  justify-content: center;
  padding: 3rem 0;

  background: white;
`;

const Form = styled.div`
  padding: 1rem;

  background: ${oc.gray[0]};
`;

const InputWrapper = styled.div`
  padding: 0.5rem 0;
`;

const Title = styled.div`
  margin-bottom: 0.5rem;
  text-align: left;
  font-size: 1rem;
  font-weight: bold;

  color: ${oc.gray[8]};
`;

const Input = styled.input`
  padding: 0.5rem;

  width: 100%;
  border: 1px solid ${oc.gray[2]};

  font-size: 1rem;
  line-height: 1.5rem;

  transition: all .25s;
  outline: none;

  &:focus {
    border: 1px solid ${oc.blue[7]};
  }
`;

Input.propTypes = {
  type: PropTypes.string,
  name: PropTypes.string,
  value: PropTypes.string,
  placeholder: PropTypes.string,
  onChange: PropTypes.func
};

const Button = styled.button`
  margin-top: 1rem;
  padding: 1rem 0;

  width: 100%;

  text-align: center;
  font-weight: 500;
  font-size: 1.2rem;

  color: white;
  background: ${props => oc[props.color][7]};
  border: 1px solid ${props => oc[props.color][10]};
  outline: none;
  cursor: pointer;

  transition: all .3s;
  &:hover {
    background: ${props => oc[props.color][6]};
  }

  &:active {
    background: ${props => oc[props.color][8]};
    border: 1px solid ${oc.blue[7]};
  }
`;

Button.propTypes = {
  color: PropTypes.string,
  onClick: PropTypes.func
};

const Login = ({ 
  width,
  form,
  error,
  innerRef,
  onChange,
  onSubmit,
  onKeyPress,
  onErrorReset
}) => (
  <div>
    <Head>
    <title>Open5gs - Login</title>
    </Head>
    <Wrapper id='nprogress-base-login' width={width}>
      <ErrorBar 
        visible={error !== null}
        message={error && error.message}
        onClose={onErrorReset} />
      <Thumbnail>
        <CircleIcon size='8rem' background={oc['blue'][6]}>
          <PersonIcon/>
        </CircleIcon>
      </Thumbnail>
      <Form>
        <InputWrapper>
          <Title>Username</Title>
          <Input 
            name="username"
            type="text"
            placeholder=""
            value={form.username} 
            onChange={onChange}
            onKeyPress={onKeyPress}
            innerRef={(comp) => {innerRef(comp)}}
          />
        </InputWrapper>
        <InputWrapper>
          <Title>Password</Title>
          <Input 
            name="password"
            type="password"
            placeholder=""
            value={form.password} 
            onChange={onChange}
            onKeyPress={onKeyPress}
          />
        </InputWrapper>
        <Button color='teal' onClick={onSubmit}>
          Login
        </Button>
      </Form>
    </Wrapper>
  </div>
);

Login.propTypes = {
  width: PropTypes.string
}

Login.defaultProps = {
  width: '360px'
}

export default Login;

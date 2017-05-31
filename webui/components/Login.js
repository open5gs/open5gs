import Session from '../lib/session';

import { Component } from 'react';
import Link from 'next/link';
import Head from 'next/head';
import Router from 'next/router';
import PropTypes from 'prop-types';

import NProgress from 'nprogress';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions} from '../lib/style-utils';

import Thumbnail from './Thumbnail';
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
  z-index: 1;

  font-size: 1.1rem;
  font-weight: 500;
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

const ErrorBar = ({ visible, message, onClick }) => visible ? (
  <ErrorWrapper>
    <ErrorMessage>
      {message}
    </ErrorMessage>
    <ErrorClose onClick={onClick}>
      <CloseIcon/>
    </ErrorClose>
  </ErrorWrapper>
) : null;

const ThumbnailWrapper = styled.div`
  display: flex;
  justify-content: center;
  padding-top: 3rem;
  padding-bottom: 3rem;

  background: white;
`;

const Form = styled.div`
  padding: 1rem;

  background: ${oc.gray[0]};
`;

const InputWrapper = styled.div`
  padding-top: 0.5rem;
  padding-bottom: 0.5rem;
`;

const Title = styled.div`
  margin-bottom: 0.3rem;
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
  padding-top: 1rem;
  padding-bottom: 1rem;

  width: 100%;

  text-align: center;
  font-weight: 500;
  font-size: 1.2rem;

  color: white;
  background: ${props => oc[props.color][7]};
  border: 1px solid ${props => oc[props.color][10]};

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

class Login extends Component {
  state = {
    error: {
      status: false,
      message: ''
    },
    account: {
      username: '',
      password: ''
    },
    session: {}
  };

  static propTypes = {
    width: PropTypes.string
  }

  static defaultProps = {
    width: '360px'
  }

  static async getInitialProps({req}) {
    const session = new Session({req})
    return {session: await session.getSession(true)}
  }

  async componentDidMount() {
    const session = new Session()
    this.setState({ 
      session: await session.getSession(true)
    });

    this.input.focus();
  }

  onAction = (e) => {
    const { 
      username,
      password
    } = this.state.account;

    NProgress.configure({ showSpinner: false });
    NProgress.start();

    const session = new Session()
    session.signin(username, password)
      .then(() => {
        NProgress.done();

        Router.push('/');
      })
      .catch(err => {
        NProgress.done();

        this.input.focus();

        this.setState({
          error: { 
            status: true, 
            message: err.message
          }
        });
      })
  }

  handleChange = (e) => {
    this.setState({ 
      account: {
        ...this.state.account,
        [e.target.name]: e.target.value 
      }
    });
  }

  handleErrorClose = (e) => {
    this.setState({
      error: { 
        status: false
      }
    });
  }

  render() {
    const { 
      handleChange,
      onAction,
      handleErrorClose
    } = this;

    const { 
      username,
      password
    } = this.state.account;

    const err = this.state.error;

    const {width} = this.props;

    return (
      <div>
        <Head>
        <title>NextEPC - Login</title>
        </Head>
        <Wrapper width={width}>
          <ErrorBar 
            visible={err.status}
            message={err.message}
            onClick={handleErrorClose} />
          <ThumbnailWrapper>
            <Thumbnail size='8rem' color={oc['blue'][6]} />
          </ThumbnailWrapper>
          <Form>
            <InputWrapper>
              <Title>Username</Title>
              <Input 
                name="username"
                type="text"
                placeholder=""
                value={username} 
                onChange={handleChange}
                innerRef={(comp) => { this.input = comp }}
              />
            </InputWrapper>
            <InputWrapper>
              <Title>Password</Title>
              <Input 
                name="password"
                type="password"
                placeholder=""
                value={password} 
                onChange={handleChange}
              />
            </InputWrapper>
            <Button color='teal' onClick={onAction}>
              Log in
            </Button>
          </Form>
        </Wrapper>
      </div>
    );
  }
}

export default Login;

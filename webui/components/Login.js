import Session from '../lib/session';

import { Component } from 'react';
import Link from 'next/link';
import Head from 'next/head';
import Router from 'next/router';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions} from '../lib/style-utils';

import Thumbnail from './Thumbnail';

const Wrapper = styled.div`
  position: fixed;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);

  width: ${props => props.width};
  ${media.mobile`
    height: 100%;
    width: 100%;
  `}
  z-index: 10;

  border-bottom: 1px solid ${oc.gray[7]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);
`;

Wrapper.propTypes = {
  width: PropTypes.string
}

const ThumbnailWrapper = styled.div`
  display: flex;
  justify-content: center;
  padding-top: 4rem;
  padding-bottom: 4rem;

  background: white;
`;

const Form = styled.div`
  padding: 1rem;

  background: ${oc.gray[0]};
`;

const Title = styled.div`
  margin-top: 1rem;
  margin-bottom: 0.4rem;

  text-align: left;
  font-weight: 600;

  color: ${oc.gray[7]};
`;

const Input = styled.input`
  padding: 0.5rem;

  width: 100%;
  border: 1px solid ${oc.gray[2]};

  font-size: 1rem;
  line-height: 1.5rem;

  &:focus {
    border: 1px solid ${oc.blue[7]};
  }
`;

Input.propTypes = {
  name: PropTypes.string,
  value: PropTypes.string,
  placeholder: PropTypes.string,
  onChange: PropTypes.func
};

const Button = styled.div`
  display: flex;
  justify-content: center;

  margin-top: 1.5rem;
  padding-top: 1rem;
  padding-bottom: 1rem;

  cursor: pointer;
  text-align: center;
  font-weight: 500;
  font-size: 1.2rem;

  color: white;
  background: ${props => oc[props.color][7]};
  border: 1px solid ${props => oc[props.color][10]};

  &:hover {
    background: ${props => oc[props.color][6]};
  }

  &:active {
    background: ${props => oc[props.color][8]};
  }
`;

Button.propTypes = {
  color: PropTypes.string
};

class Login extends Component {
  state = {
    errors: {},
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
    width: '400px'
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
  }

  onAction = (e) => {
    const { 
      username,
      password
    } = this.state.account;

    const session = new Session()
    session.signin(username, password)
    .then(() => {
      Router.push('/');
    })
    .catch(err => {
      // @FIXME Handle error
      console.log(err)
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

  render() {
    const { 
      handleChange,
      onAction
    } = this;

    const { 
      username,
      password
    } = this.state.account;

    const {width} = this.props;

    return (
      <div>
        <Head>
        <title>NextEPC - Login</title>
        </Head>
        <Wrapper width={width}>
          <ThumbnailWrapper>
            <Thumbnail size='8rem' color={oc['blue'][6]} />
          </ThumbnailWrapper>
          <Form>
            <Title>Username</Title>
            <Input 
              name="username"
              placeholder=""
              value={username} 
              onChange={handleChange}
            />
            <Title>Password</Title>
            <Input 
              name="password"
              placeholder=""
              value={password} 
              onChange={handleChange}
            />
            <Button color='teal' 
              onClick={onAction}>
              Log in
            </Button>
          </Form>
        </Wrapper>
      </div>
    );
  }
}

export default Login;

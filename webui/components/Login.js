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
import Input from './Input';
import Dimmed from './Dimmed';

const Wrapper = styled.div`
  position: fixed;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  z-index: 10;
  width: ${ props => props.width };
  ${media.mobile`
    width: calc(100% - 2rem);
  `}
`;

Wrapper.propTypes = {
  width: PropTypes.string
}

const ThumbnailWrapper = styled.div`
  padding-top: 4rem;
  padding-bottom: 4rem;
  display: flex;
  justify-content: center;

  background: white;
`;

const Form = styled.div`
  padding: 1rem;

  background: ${oc.gray[0]};
`;

const ButtonWrapper = styled.div`
  display: flex;
`;

const Button = styled.div`
  padding-top: 1rem;
  padding-bottom: 1rem;
  flex: 1;
  display: inline-block;

  cursor: pointer;
  text-align: center;
  font-weight: 500;
  font-size: 1.2rem;
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
            <Input 
              name="username"
              placeholder="Username"
              value={username} 
              onChange={handleChange}
            />
            <Input 
              name="password"
              placeholder="Password"
              value={password} 
              onChange={handleChange}
            />
          </Form>
          <ButtonWrapper>
            <Button color='teal' 
              onClick={onAction}>
              Log In
            </Button>
          </ButtonWrapper>
        </Wrapper>
        <Dimmed visible={true} />
      </div>
    );
  }
}

export default Login;

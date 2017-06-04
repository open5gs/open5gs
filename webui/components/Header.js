import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import MenuIcon from 'react-icons/lib/md/menu';
import ThumbnailIcon from './Thumbnail';

import Session from '../lib/session';

import Logout from './Logout';
import Dimmed from './Dimmed';

const Wrapper = styled.div`
  display: flex;
  align-items: center;
  height: 4rem;

  color: white;
  background: ${oc.indigo[8]};
  border-bottom: 1px solid ${oc.indigo[9]};
`;

const Menu = styled.div`
  display: inline-flex;
  margin: 0 1.5rem;

  cursor: pointer;
  font-size: 1.5rem;
`;

const Title = styled.div`
  margin: 0 0.5rem;

  font-size: 1.5rem;
  font-family: 'Ubuntu', sans-serif;
`;

const Thumbnail = styled.div`
  padding: 1rem 0;

  position: absolute;
  right: 1rem;

  cursor: pointer;
`;

class Header extends Component {
  state = {
    logout: {
      visible: false,
      dimmed: false
    }
  }

  logoutHandler = {
    show: () => {
      this.setState({
        logout: {
          ...this.state.logout,
          visible: true,
          dimmed: true
        }
      })
    },
    hide: () => {
      this.setState({
        logout: {
          ...this.state.logout,
          visible: false,
          dimmed: false
        }
      })
    },
    action: async () => {
      this.setState({
        logout: {
          ...this.state.logout,
          visible: false,
        }
      })

      const session = new Session()
      await session.signout()

      // @FIXME next/router not working reliably  so using window.location
      window.location = '/'
    }
  }

  render() {
    const {
      logoutHandler
    } = this;

    const {
      logout
    } = this.state;

    return (
      <Wrapper>
        <Menu onClick={this.props.onMenuAction}>
          <MenuIcon/>
        </Menu>
        <Title>
          Next.EPC
        </Title>
        <Thumbnail onClick={this.logoutHandler.show}>
          <ThumbnailIcon size="2rem" color={oc['pink'][4]} />
        </Thumbnail>
        <Logout 
          {...logout} 
          onHide={logoutHandler.hide}
          onAction={logoutHandler.action} />
        <Dimmed visible={logout.dimmed} />
    </Wrapper>
    )
  }
}

export default Header;
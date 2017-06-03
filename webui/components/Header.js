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
  background: ${oc.indigo[6]};
  border-bottom: 1px solid ${oc.indigo[7]};
  box-shadow: 0 3px 6px rgba(0,0,0,0.10), 0 3px 6px rgba(0,0,0,0.20);
`;

const Menu = styled.div`
  display: inline-flex;
  margin: 0 1rem;

  font-size: 2rem;
  text-align: center;
`;

const Title = styled.div`
  margin: 0 1rem;

  font-size: 1.5rem;
  font-weight: 500;
`;

const Thumbnail = styled.div`
  padding: 1rem 0;

  position: absolute;
  right: 1rem;
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
          Next, EPC
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
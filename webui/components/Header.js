import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import MenuIcon from 'react-icons/lib/md/menu';
import ThumbnailIcon from './Thumbnail';

import Session from '../lib/session';

const Wrapper = styled.div`
  position: relative;
  display: flex;
  padding-top: 1rem;
  padding-bottom: 1rem;
  z-index: 2;

  background: ${oc.indigo[6]};
  color: white;
  border-bottom: 1px solid ${oc.indigo[7]};
  box-shadow: 0 3px 6px rgba(0,0,0,0.10), 0 3px 6px rgba(0,0,0,0.20);
`;

const Menu = styled.div`
  width: 3.0rem;

  text-align: center;
  font-size: 1.5rem;
`;

const Title = styled.div`
  padding-left: 1rem;
  padding-top: 0.2rem;

  font-size: 1.5rem;
  font-weight: 200;
`;

const Thumbnail = styled.div`
  position: absolute;
  right: 1rem;

  font-size: 1.5rem;
`;

async function logout (e) {
  e.preventDefault()

  const session = new Session()
  await session.signout()

  // @FIXME next/router not working reliably  so using window.location
  window.location = '/'
}

class Header extends Component {
  render() {
    return (
      <Wrapper>
        <Menu>
          <MenuIcon/>
        </Menu>
        <Title>
          Next, EPC
        </Title>
        <Thumbnail onClick={logout}>
          <ThumbnailIcon size="2rem" color={oc['pink'][4]} />
        </Thumbnail>
    </Wrapper>
    )
  }
}

export default Header;
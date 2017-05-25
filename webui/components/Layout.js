import Package from '../package';
import Head from 'next/head';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions } from '../lib/style-utils';

import Header from './Header';

import Session from '../lib/session';
import LogoutButton from '../components/logout-button';

const Container = styled.div`
  padding-top: 60px;
`;

const Content = styled.div`
  margin: 0 auto;
  margin-top: 2rem;
  width: 1200px;
  transition: all .3s;
  position: relative;

  ${media.desktop`
    width: 990px;
  `}

  ${media.tablet`
    margin-top: 1rem;
    width: calc(100% - 2rem);
  `}

  ${media.mobile`
    margin-top: 0.5rem;
    width: calc(100% - 1rem);
  `}
`;

const Title = styled.div`
  display: block;
  text-align: left;
  width: 100%;
  color: ${oc.pink[9]};
  font-weight: bold;
  font-size: 4 rem;
`;

const Layout = ({ session, children }) => {
  const title = 'NextEPC ' + Package.version;

  return (
    <Container>
      <Head>
        <title>{title}</title>
      </Head>

      <Header/>
      <Content>
        <Title>
          Hello World
        </Title>
      </Content>

      <div>
        <p>Welcome back {session.user.username}</p>
        <LogoutButton session={session}>Log out</LogoutButton>
      </div>
    </Container>
  )
}

export default Layout;

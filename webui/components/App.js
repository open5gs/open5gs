import Package from '../package';
import Head from 'next/head';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions } from '../lib/style-utils';
import MenuIcon from 'react-icons/lib/md/menu';
import Thumbnail from './Thumbnail';

import Session from '../lib/session';
import LogoutButton from '../components/logout-button';

const Header = styled.div`
  display: flex;
  padding-top: 1rem;
  padding-bottom: 1rem;

  background: ${oc.indigo[6]};
  color: white;
  border-bottom: 1px solid ${oc.indigo[7]};
  box-shadow: 0 3px 6px rgba(0,0,0,0.10), 0 3px 6px rgba(0,0,0,0.20);
`;

const HeaderMenu = styled.div`
  width: 3.0rem;

  text-align: center;
  font-size: 1.5rem;
`;

const HeaderTitle = styled.div`
  padding-left: 1rem;
  padding-top: 0.2rem;

  font-size: 1.5rem;
  font-weight: 200;
`;

const HeaderThumbnail = styled.div`
  position: absolute;
  right: 1rem;

  font-size: 1.5rem;
`;

const Container = styled.div`
  display: flex;
  margin: 10px;
  background-color: yellow;
  border: 1px solid green;
`

const Sidebar = styled.div`
  width: 10rem;
  height: 30rem;

  margin: 10px;
  background-color: blue;
  border: 1px solid red;
`;

const Content = styled.div`
  flex: 1;
  height: 30rem;

  margin: 10px;
  background-color: blue;
  border: 1px solid red;
`;

const TempWrapper = styled.div`
`;

const App = ({ session }) => {
  const title = 'Next, EPC ' + Package.version;

  return (
    <div>
      <Head>
        <title>{title}</title>
      </Head>

      <Header>
        <HeaderMenu>
          <MenuIcon/>
        </HeaderMenu>
        <HeaderTitle>
          Next, EPC
        </HeaderTitle>
        <HeaderThumbnail>
          <Thumbnail size="2rem" color={oc['pink'][4]} />
        </HeaderThumbnail>
      </Header>
      <Container>
        <Sidebar/>
        <Content/>
      </Container>

      <TempWrapper>
      <p>Welcome back {session.user.username}</p>
      <LogoutButton session={session}>Log out</LogoutButton>
      </TempWrapper>
    </div>
  )
}

export default App;
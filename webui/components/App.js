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

const BodyContainer = styled.div`
  display: flex;
`

const Sidebar = styled.div`
  position: relative;
  width: 16rem;
  z-index: 1;

  background-color: ${oc.indigo[3]};
  border-right: 1px solid ${oc.indigo[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);
`;

const ContentContainer = styled.div`
  flex: 1;
`;

const Content = styled.div`
  padding: 2rem;
`;

const HelloWorld = styled.div`
  display: flex;
  align-items: center;
  justify-content: center;
  height : 35rem;

  font-size: 4rem;
  color: ${oc.gray[5]};

  border-top: 1px solid ${oc.gray[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);

  background-color: white;
`;

const Footer = styled.div`
  padding: 1rem 1rem 1rem 0;

  text-align: right;
  font-size: 1rem;
  color: ${oc.gray[7]};

  border-top: 1px solid ${oc.gray[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);

  background-color: white;
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
      <BodyContainer>
        <Sidebar/>
        <ContentContainer>
          <Content>
            <HelloWorld>
              Hello, World
            </HelloWorld>
          </Content>
          <Footer>
            Copyright © 2017 NextEPC Group. All rights reserved.
          </Footer>
        </ContentContainer>
      </BodyContainer>

      <TempWrapper>
      <p>Welcome back {session.user.username}</p>
      <LogoutButton session={session}>Log out</LogoutButton>
      </TempWrapper>
    </div>
  )
}

export default App;
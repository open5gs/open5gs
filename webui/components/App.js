import Package from '../package';
import Head from 'next/head';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions } from '../lib/style-utils';

import Header from './Header';

import Session from '../lib/session';

const BodyContainer = styled.div`
  display: flex;
  height: calc(100vh - 4rem);
`

const Sidebar = styled.div`
  width: 16rem;
  z-index: 1;

  background-color: ${oc.indigo[3]};
  border-right: 1px solid ${oc.indigo[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);
`;

const ContentContainer = styled.div`
  flex: 1;
  padding: 1rem;
`;

const HelloWorld = styled.div`
  display: flex;
  align-items: center;
  justify-content: center;

  font-size: 4rem;
  line-height: 8rem;
  color: ${oc.gray[5]};

  border-top: 1px solid ${oc.gray[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);

  background-color: white;
`;

const App = ({ session }) => {
  const title = 'Next, EPC ' + Package.version;

  return (
    <div>
      <Head>
        <title>{title}</title>
      </Head>

      <Header/>
      <BodyContainer>
        <Sidebar/>
        <ContentContainer>
          <HelloWorld>
            Hello, World
          </HelloWorld>
        </ContentContainer>
      </BodyContainer>
    </div>
  )
}

export default App;

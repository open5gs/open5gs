import PropTypes from 'prop-types';

import styled from 'styled-components';
import { media, transitions} from '../lib/style-utils';
import oc from 'open-color';

import Head from 'next/head';

import HeaderContainer from '../containers/HeaderContainer';
import SidebarContainer from '../containers/SidebarContainer';

const Main = styled.div`
  display: flex;
  height: calc(100vh - 4rem);
`

const propTypes = {
  title: PropTypes.string;
}

const Layout = ({title, children}) => (
  <div>
    <Head>
      <title>{title}</title>
    </Head>
    <HeaderContainer/>
    <Main>
      <SidebarContainer/>
      {children}
    </Main>
  </div>
)

Layout.propTypes = propTypes;

Layout.Content = styled.div`
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


export default Layout;
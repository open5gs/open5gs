import PropTypes from 'prop-types';
import Head from 'next/head';

import styled from 'styled-components';
import oc from 'open-color';

import HeaderContainer from 'containers/HeaderContainer';
import SidebarContainer from 'containers/SidebarContainer';

import Package from '../../../package';

const Body = styled.div`
  display: flex;
  height: calc(100vh - 4rem);
`

const propTypes = {
  title: PropTypes.string
}

const defaultProps = {
  title: `Next.EPC ${Package.version}`
}

const Layout = ({ title, children }) => (
  <div>
    <Head>
      <title>{title}</title>
    </Head>
    <HeaderContainer/>
    <Body>
      <SidebarContainer/>
      {children}
    </Body>
  </div>
)

Layout.propTypes = propTypes;
Layout.defaultProps = defaultProps;

const ContainerWrapper = styled.div`
  flex: 1;
  overflow-y: scroll;
`;

Layout.Container = ({visible, children}) => visible ? (
  <ContainerWrapper>
    {children}
  </ContainerWrapper>
) : null;

Layout.Content = styled.div`
  width: 100%;
  height: 100%;
  
  background: ${p => p.background ? p.background : `white`};
`;

export default Layout;
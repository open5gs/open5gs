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
`;

Layout.Container = ({visible, children}) => visible ? (
  <ContainerWrapper>
    {children}
  </ContainerWrapper>
) : null;

/* this is a temporal CSS */
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
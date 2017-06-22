import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import MenuIcon from 'react-icons/lib/md/menu';
import { ThumbnailIcon, Tooltip } from 'components';

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
  right: 2rem;

  cursor: pointer;
`;

const propTypes = {
  onSidebarToggle: PropTypes.func.isRequired,
  onLogoutRequest: PropTypes.func.isRequired
}

const Header = ({ onSidebarToggle, onLogoutRequest }) => (
  <Wrapper>
    <Menu onClick={onSidebarToggle}>
      <MenuIcon/>
    </Menu>
    <Title>
      Next.EPC
    </Title>
    <Thumbnail onClick={onLogoutRequest}>
      <Tooltip bottom content='Logout' width="80px">
        <ThumbnailIcon size="2rem" color={oc['pink'][4]} />
      </Tooltip>
    </Thumbnail>
  </Wrapper>
)

Header.propTypes = propTypes;

export default Header;
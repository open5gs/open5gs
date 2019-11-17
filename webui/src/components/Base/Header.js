import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import { CircleIcon, Tooltip } from 'components';
import MenuIcon from 'react-icons/lib/md/menu';
import PersonIcon from 'react-icons/lib/md/person';

const Wrapper = styled.div`
  display: flex;
  align-items: center;
  height: 4rem;

  color: white;
  background: ${oc.indigo[9]};
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
      Open5GS
    </Title>
    <Thumbnail onClick={onLogoutRequest}>
      <Tooltip bottom content='Logout' width="60px">
        <CircleIcon size="2rem" background={oc['pink'][4]}>
          <PersonIcon/>
        </CircleIcon>
      </Tooltip>
    </Thumbnail>
  </Wrapper>
)

Header.propTypes = propTypes;

export default Header;

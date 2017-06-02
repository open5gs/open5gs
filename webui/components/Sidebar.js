import PropTypes from 'prop-types';

import styled from 'styled-components';
import { media, transitions} from '../lib/style-utils';
import oc from 'open-color';

import PdnIcon from 'react-icons/lib/md/settings-remote';
import UserIcon from 'react-icons/lib/md/supervisor-account';

const Menu = styled.div`
  display: block;
  z-index: 1;
  width: ${p => p.toggled ? '0' : p.width };
  transition: width .2s ease-in-out;

  ${media.mobile`
    width: 100%;
    height: ${p => p.toggled ? '100%' : '0'};
    transition: height .2s ease-in-out;
  `}

  background-color: ${oc.indigo[3]};
  border-right: 1px solid ${oc.indigo[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.1), 3px 3px 6px rgba(0,0,0,0.2);
`;

const ItemWrapper = styled.div`
  display: flex;
  align-items: center;
  margin: 0.5rem;
  padding : 0.5rem;

  transition: all .3s;

  color: white;
  &:hover {
    background: ${oc.indigo[2]}
  }

  &:active {
    background: ${oc.indigo[4]}
  }
`;

const IconWrapper = styled.div`
  display: inline-flex;
  padding-left: 1rem;
  font-size: 1.5rem;

  text-align: center;
`;

const TitleWrapper = styled.div`
  padding-left: 2rem;
  font-size: 1.2rem;
`;

const Item = ({ icon, title }) => (
  <ItemWrapper>
    <IconWrapper>{icon}</IconWrapper>
    <TitleWrapper>{title}</TitleWrapper>
  </ItemWrapper>
)

const Sidebar = ({ toggled, width }) => (
  <Menu toggled={toggled} width={width}>
    <Item icon={<PdnIcon/>} title='PDN'/>
    <Item icon={<UserIcon/>} title='User'/>
  </Menu>
)

Sidebar.propTypes = {
  toggled: PropTypes.bool,
  width: PropTypes.string
};

Sidebar.defaultProps = {
  toggled: false,
  width: '16rem'
};

export default Sidebar;

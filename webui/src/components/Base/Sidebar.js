import PropTypes from 'prop-types';

import styled from 'styled-components';
import { media, transitions } from 'helpers/style-utils';
import oc from 'open-color';

import SubscriberIcon from 'react-icons/lib/md/person-add';
import ProfileIcon from 'react-icons/lib/md/content-copy';
import AccountIcon from 'react-icons/lib/md/vpn-key';

const Menu = styled.div`
  display: block;
  width: ${p => p.visible ? p.width : '0' };
  transition: width .2s ease-in-out;
  overflow: hidden;

  position: relative;
  z-index: 1;

  ${media.mobile`
    position: absolute;
    top: 4rem;
    left: 0;
    width: ${p => p.visible ? '100%' : '0'};
    height: ${p => p.visible ? '100%' : '0'};
    transition: height .2s ease-in-out;
  `}

  background-color: ${oc.indigo[5]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.1), 3px 3px 6px rgba(0,0,0,0.2);
`;

const StyledItem = styled.div`
  display: flex;
  align-items: center;
  padding : 1rem;

  transition: all .3s;

  cursor: pointer;
  color: white;
  background: ${p => p.active ? oc.indigo[7] : oc.indigo[5]};

  border-left: ${p => p.active ? `12px solid ${oc.indigo[6]}` :
    `12px solid ${oc.indigo[4]}`};

  &:hover {
    background: ${p => p.active ? oc.indigo[7] : oc.indigo[6]};
  }
`;

const Icon = styled.div`
  display: inline-flex;
  padding-left: 1rem;
  font-size: 1.5rem;
`;

const Title = styled.div`
  padding-left: 2rem;
  font-size: 1.2rem;
`;

const Item = ({ children, selected, name, onSelect }) => (
  <StyledItem
    onClick={() => onSelect(name)}
    active={name===selected}>
    {children}
  </StyledItem>
)

const propTypes = {
  isOpen: PropTypes.bool,
  width: PropTypes.string,
  selectedView: PropTypes.string,
  onSelectView: PropTypes.func
}

const defaultProps = {
  width: "16rem",
}

const Sidebar = ({ isOpen, width, selectedView, onSelectView }) => (
  <Menu visible={isOpen} width={width}>
    <Item name="subscriber" selected={selectedView} onSelect={onSelectView}>
      <Icon><SubscriberIcon/></Icon>
      <Title>Subscriber</Title>
    </Item>
    <Item name="profile" selected={selectedView} onSelect={onSelectView}>
      <Icon><ProfileIcon/></Icon>
      <Title>Profile</Title>
    </Item>
    <Item name="account" selected={selectedView} onSelect={onSelectView}>
      <Icon><AccountIcon/></Icon>
      <Title>Account</Title>
    </Item>
  </Menu>
)

Sidebar.propTypes = propTypes;
Sidebar.defaultProps = defaultProps;

export default Sidebar;

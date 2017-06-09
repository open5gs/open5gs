import PropTypes from 'prop-types';

import styled from 'styled-components';
import { media, transitions } from 'helpers/style-utils';
import oc from 'open-color';

import PdnIcon from 'react-icons/lib/md/cast';
import UserIcon from 'react-icons/lib/md/supervisor-account';
import Test1Icon from 'react-icons/lib/md/ac-unit'
import Test2Icon from 'react-icons/lib/md/access-alarm'
import Test3Icon from 'react-icons/lib/md/3d-rotation'

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

  background-color: ${oc.indigo[6]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.1), 3px 3px 6px rgba(0,0,0,0.2);
`;

const StyledItem = styled.div`
  display: flex;
  align-items: center;
  padding : 1rem;

  transition: all .3s;

  cursor: pointer;
  color: white;
  background: ${p => p.active ? oc.indigo[5] : oc.indigo[6]};

  border-left: ${p => p.active ? `12px solid ${oc.indigo[3]}` : 
    `12px solid ${oc.indigo[7]}`};

  &:hover {
    background: ${p => p.active ? oc.indigo[5] : oc.indigo[7]};
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
  selectedView: "pdn"
}

const Sidebar = ({ isOpen, width, selectedView, onSelectView }) => (
  <Menu visible={isOpen} width={width}>
    <Item name="pdn" selected={selectedView} onSelect={onSelectView}>
      <Icon><PdnIcon/></Icon>
      <Title>PDN</Title>
    </Item>
    <Item name="user" selected={selectedView} onSelect={onSelectView}>
      <Icon><UserIcon/></Icon>
      <Title>User</Title>
    </Item>
    <Item name="test1" selected={selectedView} onSelect={onSelectView}>
      <Icon><Test1Icon/></Icon>
      <Title>Test111111111</Title>
    </Item>
    <Item name="test2" selected={selectedView} onSelect={onSelectView}>
      <Icon><Test2Icon/></Icon>
      <Title>Testaaaaaaaa</Title>
    </Item>
    <Item name="test3" selected={selectedView} onSelect={onSelectView}>
      <Icon><Test3Icon/></Icon>
      <Title>Test@#!@#!@#</Title>
    </Item>
  </Menu>
)

Sidebar.propTypes = propTypes;
Sidebar.defaultProps = defaultProps;

export default Sidebar;

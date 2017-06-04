import PropTypes from 'prop-types';

import styled from 'styled-components';
import { media, transitions} from '../lib/style-utils';
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
  padding: 1rem 0;

  ${media.mobile`
    position: absolute;
    top: 4rem;
    left: 0;
    width: ${p => p.visible ? '100%' : '0'};
    height: ${p => p.visible ? '100%' : '0'};
    transition: height .2s ease-in-out;
    z-index: 1;
  `}

  background-color: ${oc.indigo[6]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.1), 3px 3px 6px rgba(0,0,0,0.2);
`;

const StyledItem = styled.div`
  display: flex;
  align-items: center;
  padding : 1rem;

  transition: all .3s;

  color: white;
  background: ${p => p.active ? oc.indigo[5] : oc.indigo[6]};

  border-left: ${p => p.active ? `12px solid ${oc.indigo[9]}` : null};

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

const Sidebar = ({ visible, width, selected, onSelect }) => (
  <Menu visible={visible} width={width}>
    <Item name="PDN" selected={selected} onSelect={onSelect}>
      <Icon><PdnIcon/></Icon>
      <Title>PDN</Title>
    </Item>
    <Item name="User" selected={selected} onSelect={onSelect}>
      <Icon><UserIcon/></Icon>
      <Title>User</Title>
    </Item>
    <Item name="Test1" selected={selected} onSelect={onSelect}>
      <Icon><Test1Icon/></Icon>
      <Title>Test111111111</Title>
    </Item>
    <Item name="Test2" selected={selected} onSelect={onSelect}>
      <Icon><Test2Icon/></Icon>
      <Title>Testaaaaaaaa</Title>
    </Item>
    <Item name="Test3" selected={selected} onSelect={onSelect}>
      <Icon><Test3Icon/></Icon>
      <Title>Test@#!@#!@#</Title>
    </Item>
  </Menu>
)

Sidebar.propTypes = {
  visible: PropTypes.bool,
  width: PropTypes.string,
  selected: PropTypes.string,
  onSelect: PropTypes.func
};

Sidebar.defaultProps = {
  visible: false,
  width: "16rem",
  selected: "PDN"
};

export default Sidebar;

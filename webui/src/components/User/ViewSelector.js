import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

const Wrapper = styled.div`
  height: 3rem;
  background: white;
  width: 100%;

  position: relative;    
`;

const ItemContainer = styled.div`
  height: 100%;
  width: 100%;
  display: flex;
`;

const StyledItem = styled.div`
  height: 100%;

  flex: 1; 

  display: flex;
  align-items: center;
  justify-content: center;
  
  color: ${ p => p.active ? oc.gray[9] : oc.gray[6] };

  font-size: 1.2rem;
  cursor: pointer;

  &:hover {
    background: ${p => p.active ? `white` : oc.gray[0]};
  }
`;

StyledItem.propTypes = {
  active: PropTypes.bool
}

const Bar = styled.div`
  position: absolute;
  bottom: 0px;
  height: 3px;
  width: 50%;

  background: ${oc.pink[6]};

  transition: ease-in .25s;

  transform: ${props => props.right ? 'translateX(100%)' : 'none'};
`;

Bar.propTypes = {
  right: PropTypes.bool
}

const Item = ({children, selected, name, onSelect}) => (
  <StyledItem 
    onClick={() => onSelect(name)}
    active={name===selected}>
    {children}
  </StyledItem>
);

Item.propTypes = {
  selected: PropTypes.string,
  name: PropTypes.string,
  onSelect: PropTypes.func
};

const ViewSelector = ({selected, onSelect}) => (
  <Wrapper>
    <ItemContainer>
      <Item 
        selected={selected}
        name="user" 
        onSelect={onSelect}>
        User
      </Item>
      <Item 
        selected={selected} 
        name="profile" 
        onSelect={onSelect}>
        Profile
      </Item>
    </ItemContainer>
    <Bar right={selected==='profile'}/>
  </Wrapper>
);

ViewSelector.propTypes = {
  selected: PropTypes.string,
  onSelect: PropTypes.func
}

export default ViewSelector;

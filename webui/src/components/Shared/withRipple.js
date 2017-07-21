import React, { Component } from 'react';
import styled from 'styled-components';

function getAlignement(props) {
  if (props.align === 'center') return '0px auto';
  if (props.align === 'right') return '0px 0px 0px auto';
  if (props.align === 'left') return '0px auto 0px 0px';
  return '0px';
}

const RippleWrapper = styled.div`
  position: relative;
  overflow: hidden;
  transform: translate3d(0, 0, 0);
  display: inline-block;
  align-self: flex-start;
  margin: ${props => getAlignement(props)};

  &:after {
    content: "";
    display: block;
    position: absolute;
    width: 100%;
    height: 100%;
    top: 0;
    left: 0;
    pointer-events: none;
    background-image: radial-gradient(circle, #000 10%, transparent 10.01%);
    background-repeat: no-repeat;
    background-position: 50%;
    transform: scale(10,10);
    opacity: 0;
    transition: transform .5s, opacity 1s;
  }

  &:active:after {
    transform: scale(0,0);
    opacity: .2;
    transition: 0s;
  }
`;


/* eslint-disable react/prefer-stateless-function */
const withRipple = (Comp) => {
  return class RippleProvider extends Component {
    render() {
      return (
        <RippleWrapper align={this.props.align}>
          <Comp {...this.props} />
        </RippleWrapper>
      );
    }
  };
};
/* eslint-enable react/prefer-stateless-function */

export default withRipple;

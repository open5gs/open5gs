import React from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import withRipple from './withRipple';

function getBgColor(props) {
  let color = oc.indigo[7];
  let colorDark = oc.indigo[8];

  // Determine color based on props
  if (props.secondary) {
    color = oc.violet[7];
    colorDark = oc.violet[8];
  }
  if (props.info) {
    color = oc.gray[7];
    colorDark = oc.gray[8];
  }
  if (props.danger) {
    color = oc.red[7];
    colorDark = oc.red[8];
  }
  if (props.success) {
    color = oc.lime[7];
    colorDark = oc.lime[8];
  }
  if (props.outline || props.clear) {
    color = 'transparent';
    colorDark = 'transparent';
  }

  /* eslint-disable max-len */
  return `background: ${color}; background: linear-gradient(to bottom, ${color} 0%, ${colorDark} 100%);`;
  /* eslint-enable max-len */
}

function getHoverColor(props) {
  let color = oc.indigo[8];
  if (props.secondary) color = oc.violet[8];
  if (props.info) color = oc.gray[8];
  if (props.danger) color = oc.red[8];
  if (props.success) color = oc.lime[8];
  if (props.clear) color = 'transparent';

  return color;
}

function getActiveColor(props) {
  let color = oc.indigo[9];
  if (props.secondary) color = oc.violet[9];
  if (props.info) color = oc.gray[9];
  if (props.danger) color = oc.red[9];
  if (props.success) color = oc.lime[9];

  return color;
}

function getColor(props) {
  if (props.primary) return oc.indigo[7];
  if (props.secondary) return oc.violet[7];
  if (props.info) return oc.gray[7];
  if (props.danger) return oc.red[7];
  if (props.success) return oc.lime[7];
  return oc.indigo[7]; // default
}

const ButtonWrapper = styled.button`
  outline: none;
  font-size: 1rem;
  padding: 8px 16px;
  font-weight: normal;
  width: ${props => props.w || 'auto'};
  text-shadow: 1px 1px 2px rgba(0,0,0,0.1);
  border-style: solid;
  border-width: ${props => props.outline && !props.clear ? '1px' : '0px'};
  border-radius: 4px;
  border-color: ${props => getColor(props)};
  color: ${props => props.outline || props.clear ? getColor(props) : '#fff'};
  ${props => (!props.outline && !props.clear && !props.flat) &&
    'box-shadow: 1px 3px 6px rgba(0,0,0,0.1);'
  }
  ${props => getBgColor(props)};
  ${props => props.disabled && 'opacity: 0.5; cursor: not-allowed;'};
  ${props => props.small && 'padding: 4px 8px;'}
  ${props => props.large && 'padding: 12px 20px;'}
  ${props => props.small && 'font-size: 0.8rem;'}
  ${props => props.large && 'font-size: 1.5rem;'}

  &:hover {
    background: ${props => getHoverColor(props)};
    ${props => !props.clear && 'color: #fff;'}
    ${props => !props.disabled && 'cursor: pointer;'}
  }
  &:active {
    background: ${props => !props.clear && getActiveColor(props)};
    color: ${props => props.clear && getActiveColor(props)};
  }
`;

const ButtonContent = styled.div`
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: center;
  width: 100%;
  ${props => props.disabled && 'pointer-events: none;'};
`;

const Button = ({ children, ...rest }) => (
  <ButtonWrapper {...rest}>
    <ButtonContent disabled={rest.disabled}>
      {children}
    </ButtonContent>
  </ButtonWrapper>
);

Button.propTypes = {
  children: PropTypes.any,
};

export default withRipple(Button);

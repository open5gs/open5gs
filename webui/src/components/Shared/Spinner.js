import React from 'react';
import styled from 'styled-components';
import oc from 'open-color';
import PropTypes from 'prop-types';

const Wrapper = styled.div`
  padding : 8rem;
  text-align: center;
`;

const propTypes = {
  visible: PropTypes.bool,
  color: PropTypes.string,
  size: PropTypes.string,
}

const defaultProps = {
  visible: true,
  color: "blue",
  size: "64"
}

const Spinner = ({visible, color, size}) => visible ? (
  <Wrapper>
    <svg width={size} height={size} viewBox="0 0 38 38" xmlns="http://www.w3.org/2000/svg" stroke={color}>
    <g fill="none" fillRule="evenodd">
      <g transform="translate(1 1)" strokeWidth="2">
        <circle strokeOpacity=".5" cx="18" cy="18" r="18"/>
        <path d="M36 18c0-9.94-8.06-18-18-18">
          <animateTransform
            attributeName="transform"
            type="rotate"
            from="0 18 18"
            to="360 18 18"
            dur="1s"
            repeatCount="indefinite"/>
        </path>
      </g>
    </g>
    </svg>
  </Wrapper>
) : null;

Spinner.propTypes = propTypes;
Spinner.defaultProps = defaultProps;

export default Spinner;
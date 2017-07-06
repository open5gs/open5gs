import PropTypes from 'prop-types';
import React from 'react';
import styled, { css } from 'styled-components';
import oc from 'open-color';

const textCss = css`
  color: ${props => props.color || oc.gray[9]};
  ${props => props.nomargin && 'margin: 0;'}
  ${props => props.sub && 'font-weight: 400;'}
`;
const HeadingLg = styled.h1`
  ${textCss}
`;
const HeadingMd = styled.h2`
  ${textCss}
`;
const HeadingSm = styled.h3`
  ${textCss}
`;
const HeadingXs = styled.h4`
  ${textCss}
`;
const HeadingTiny = styled.h5`
  ${textCss}
`;

const Heading = ({ el, children, ...rest }) => {
  if (el === 'h1') return (<HeadingLg {...rest}>{children}</HeadingLg>);
  if (el === 'h2') return (<HeadingMd {...rest}>{children}</HeadingMd>);
  if (el === 'h3') return (<HeadingSm {...rest}>{children}</HeadingSm>);
  if (el === 'h4') return (<HeadingXs {...rest}>{children}</HeadingXs>);
  if (el === 'h5') return (<HeadingTiny {...rest}>{children}</HeadingTiny>);
  return (<HeadingLg {...rest}>{children}</HeadingLg>);
};

Heading.propTypes = {
  el: PropTypes.string,
  color: PropTypes.string,
  children: PropTypes.any,
};

export default Heading;

import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

const Wrapper = styled.div`
  width: ${p => p.size};
  height: ${p => p.size};
  margin: 0 auto;

  display: flex;
  align-items: center;
  justify-content: center;

  border-radius: calc(${p => p.size} * 0.5 );
  font-size: calc(${p => p.size} * 0.75);

  background: ${p => p.background};
  color: ${p => p.color};
`;

Wrapper.propTypes = {
  size: PropTypes.string,
  background: PropTypes.string,
  color: PropTypes.string
};

const CircleIcon = ({ children, size, background, color }) => (
  <Wrapper size={size} background={background} color={color}>
    {children}
  </Wrapper>
)

CircleIcon.propTypes = {
  size: PropTypes.string,
  background: PropTypes.string,
  color: PropTypes.string
};

CircleIcon.defaultProps = {
  size: '4rem',
  background: '#000',
  color: 'white'
};

export default CircleIcon;
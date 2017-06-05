import PropTypes from 'prop-types';

import styled from 'styled-components';

const Wrapper = styled.div`
  flex: 1;
`;

const Container = ({visible, children}) => visible ? (
  <Wrapper>
    {children}
  </Wrapper>
) : null;

Container.propTypes = {
  visible: PropTypes.bool
};

export default Container;
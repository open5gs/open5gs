import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions } from 'helpers/style-utils';

import { Layout } from 'components';
import Item from './Item';

const Wrapper = styled.div`
  display: block;
  
  ${media.mobile`
      margin-top: 0.25rem;
  `}
`

const propTypes = {
  subscribers: PropTypes.arrayOf(PropTypes.object)
}

const List = ({ subscribers }) => (
  <Wrapper>
    {subscribers.map(subscriber =>
      <Item subscriber={subscriber} />
    )}
  </Wrapper>
)

List.propTypes = propTypes;

export default List;
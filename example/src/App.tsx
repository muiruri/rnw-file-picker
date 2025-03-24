import { useState } from 'react';
import { Text, View, StyleSheet, Button } from 'react-native';
import { pickFile } from 'rnw-file-picker';

export default function App() {
  const [path, setPath] = useState('');

  const onPickFile = async () => {
    const p = await pickFile(['.txt', '.pdf'], '');
    setPath(JSON.stringify(p));
  };

  return (
    <View style={styles.container}>
      <Button title="Open File" onPress={() => onPickFile()} />
      <Text>{path}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
});

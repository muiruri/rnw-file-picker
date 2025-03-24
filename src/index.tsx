import RNWFilePicker from './NativeRNWFilePicker';

export function pickFile(types: string[], destFolder: string): Promise<Object> {
  return RNWFilePicker.pickFile(types, destFolder);
}
